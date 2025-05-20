#include "ccpch.h"
#include "Scene.h"

// Cataclysm
#include "Cataclysm/Core/Log.h"
#include "Cataclysm/ECS/Components.h"
#include "Cataclysm/ECS/Entity.h"
#include "Cataclysm/ECS/ScriptableEntity.h"
#include "Cataclysm/Math/Math.h"
#include "Cataclysm/Scripting/ScriptEngine.h"
#include "Cataclysm/Renderer/Vesuvius/Vesuvius.h"
#include "Cataclysm/Physics/Physics2D.h"
#include "Cataclysm/Physics/ContactListener2D.h"
#include "Cataclysm/Audio/AudioEngine.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

// glm
#include <glm/glm.hpp>

namespace Cataclysm
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);
				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}
	
	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();

		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
			newEntity.GetComponent<IDComponent>().Enabled = srcSceneRegistry.get<IDComponent>(e).Enabled;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		// Copy the parent map
		auto& srcParentMap = other->GetEntityParentMap();
		auto& dstParentMap = newScene->GetEntityParentMap();

		// Copy parent relationships
		for (const auto& [childUUID, parentUUID] : srcParentMap)
		{
			entt::entity newChild = enttMap[childUUID];  // Get the new entity for the child (entt::entity)
			entt::entity newParent = enttMap[parentUUID]; // Get the new entity for the parent (entt::entity)

			// Now set the parent-child relationship using entt::entity
			newScene->SetParent(childUUID, parentUUID);  // Set parent in the new scene
		}

		// Copy the children map (if necessary, SetParent will handle it)
		auto& srcChildrenMap = other->GetEntityChildrenMap();
		auto& dstChildrenMap = newScene->GetEntityChildrenMap();

		// Copy child relationships
		for (const auto& [parentUUID, childUUID] : srcChildrenMap)
		{
			entt::entity newParent = enttMap[parentUUID];  // Get the new parent entity (entt::entity)
			entt::entity newChild = enttMap[childUUID];    // Get the new child entity (entt::entity)

			// Insert into the children map of the new scene
			newScene->GetEntityChildrenMap().insert({ parentUUID, childUUID});
		}

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string & name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		UUID id = entity.GetUUID();

		// 1. If the entity has a parent, remove this entity from its parent's children
		auto parentIt = m_EntityParentMap.find(id);
		if (parentIt != m_EntityParentMap.end())
		{
			UUID parentID = parentIt->second;

			// Remove this entity from the parent's children multimap
			auto [begin, end] = m_EntityChildrenMap.equal_range(parentID);
			for (auto it = begin; it != end; ++it)
			{
				if (it->second == id)
				{
					m_EntityChildrenMap.erase(it);
					break;
				}
			}

			// Remove from parent map
			m_EntityParentMap.erase(parentIt);
		}

		// 2. If the entity has children, remove their parent entries and optionally reparent or delete them
		auto [childBegin, childEnd] = m_EntityChildrenMap.equal_range(id);
		std::vector<UUID> childrenToRemove;
		for (auto it = childBegin; it != childEnd; ++it)
		{
			childrenToRemove.push_back(it->second);
		}

		// Remove all child -> parent links
		for (UUID childID : childrenToRemove)
		{
			m_EntityParentMap.erase(childID);

			// Recursively delete
			DestroyEntity(GetEntityByUUID(childID));
		}

		// Remove this entity's children map entry
		m_EntityChildrenMap.erase(id);

		// 3. Destroy from registry
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;

		OnPhysics2DStart();

		// Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			// Instantiate all script entities

			auto view = m_Registry.view<MonoScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;

		OnPhysics2DStop();
		ScriptEngine::OnRuntimeStop();

		// Audio
		{
			auto view = m_Registry.view<AudioSourceComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				AudioSourceComponent& asc = entity.GetComponent<AudioSourceComponent>();

				if (!asc.AudioClip && !asc.FilePath.empty())
				{
					asc.AudioClip = AudioEngine::LoadClip(asc.FilePath);
					if (asc.AudioClip)
						asc.AudioClip->SetLooping(asc.Loop);
				}

				if (asc.PlayOnStart)
				{
					AudioEngine::Stop(asc.AudioClip);
					asc.IsPlaying = true;
				}
			}
		}
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	static bool IsMatrixValid(const glm::mat4& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			if (glm::any(glm::isnan(mat[i])) || glm::any(glm::isinf(mat[i])))
				return false;
		}
		return glm::determinant(mat) != 0.0f;
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{

			for (auto entity : m_Registry.view<TransformComponent>())
			{
				Entity e = { entity, this };
				if (GetParent(e.GetUUID()) == 0)
					UpdateTransformRecursive(e); // now truly recursive
			}


			// Audio
			{
				auto view = m_Registry.view<AudioSourceComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					AudioSourceComponent& asc = entity.GetComponent<AudioSourceComponent>();

					if (!asc.AudioClip && !asc.FilePath.empty())
					{
						asc.AudioClip = AudioEngine::LoadClip(asc.FilePath);
						if (asc.AudioClip)
							asc.AudioClip->SetLooping(asc.Loop);
					}

					if (asc.PlayOnStart)
					{
						AudioEngine::Play(asc.AudioClip);
						asc.IsPlaying = true;
					}
				}
			}

			// Update Scripts
			{
				// C# Entity OnUpdate
				auto view = m_Registry.view<MonoScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					if (IsEnabled(entity))
					{
						ScriptEngine::OnUpdateEntity(entity, ts);

					}
				}

				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
					{
						// TODO: Move to Scene::OnScenePlay
						if (!nsc.Instance)
						{
							nsc.Instance = nsc.InstantiateScript();
							nsc.Instance->m_Entity = Entity{ entity, this };
							nsc.Instance->OnCreate();
						}

						nsc.Instance->OnUpdate(ts);
					});
			}

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// 1. Apply physics results to local transform (no recursion yet)
				auto view = m_Registry.view<Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& id = entity.GetComponent<IDComponent>();
					if (!IsEnabled(entity)) continue;

					if (!entity.HasComponent<TransformComponent>())
					{
						std::string error = "[Scene::OnUpdateRuntime] Entity '" + entity.GetName() + "' does not contain component <TransformComponent>";
						RuntimeErrorHit(error);
					}

					/*
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
					b2Body* body = (b2Body*)rb2d.RuntimeBody;

					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
					*/

					
					auto& tc = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;

					if (rb2d.Type == Rigidbody2DComponent::BodyType::Dynamic)
					{
						const b2Vec2 position = body->GetPosition();
						const float angle = body->GetAngle();

						glm::vec3 physicsTranslation = { position.x, position.y, tc.Translation.z };
						glm::vec3 physicsRotation = { 0.0f, 0.0f, angle };

						glm::mat4 newGlobalTransform =
							glm::translate(glm::mat4(1.0f), physicsTranslation) *
							glm::rotate(glm::mat4(1.0f), physicsRotation.z, glm::vec3(0, 0, 1)) *
							glm::scale(glm::mat4(1.0f), tc.Scale); // Use original local scale

						SetWorldTransform(entity.GetUUID(), newGlobalTransform);
					}

				}

				// 2. Recompute global transforms after physics
				for (auto entity : m_Registry.view<TransformComponent>())
				{
					Entity e = { entity, this };
					if (GetParent(e.GetUUID()) == 0)
						UpdateTransformRecursive(e);
				}
			}

			// Process collisions
			const auto& collisions = m_ContactListener->GetCollisions();
			for (const auto& collision : collisions)
			{
				Entity entityA = GetEntityByUUID(collision.EntityA);
				Entity entityB = GetEntityByUUID(collision.EntityB);

				if (entityA.HasComponent<MonoScriptComponent>())
					ScriptEngine::OnCollisionEnter2D(entityA, entityB);

				if (entityB.HasComponent<MonoScriptComponent>())
					ScriptEngine::OnCollisionEnter2D(entityB, entityA);
			}

			const auto& endCollisions = m_ContactListener->GetEndCollisions();
			for (const auto& endCollision : endCollisions)
			{
				Entity entityA = GetEntityByUUID(endCollision.EntityA);
				Entity entityB = GetEntityByUUID(endCollision.EntityB);

				if (entityA.HasComponent<MonoScriptComponent>())
					ScriptEngine::OnCollisionExit2D(entityA, entityB);

				if (entityB.HasComponent<MonoScriptComponent>())
					ScriptEngine::OnCollisionExit2D(entityB, entityA);
			}


			m_ContactListener->Clear();
			m_ContactListener->ClearEndCollisions();
		}
		else
		{
			// Audio
			{
				auto view = m_Registry.view<AudioSourceComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					AudioSourceComponent& asc = entity.GetComponent<AudioSourceComponent>();

					if (!asc.AudioClip && !asc.FilePath.empty())
					{
						asc.AudioClip = AudioEngine::LoadClip(asc.FilePath);
						if (asc.AudioClip)
							asc.AudioClip->SetLooping(asc.Loop);
					}

					if (asc.IsPlaying)
					{
						AudioEngine::Pause(asc.AudioClip);
						asc.IsPlaying = false;
					}
				}
			}
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		Entity camEntity;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& tag = entity.GetComponent<TagComponent>();
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(e);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					camEntity = entity;
					break;
				}
			}
		}

		if (mainCamera && IsEnabled(camEntity))
		{
			Vesuvius::BeginScene(*mainCamera, cameraTransform);

			// Draw Sprites
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto e : group)
				{
					Entity entity = { e, this };
					if (IsEnabled(entity))
					{
						auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
						Vesuvius::DrawSprite(transform.GlobalTransform, sprite, (int)e);
					}
				}
			}

			// Draw Circles
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					if (IsEnabled(entity))
					{
						auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(e);
						Vesuvius::DrawCircle(transform.GlobalTransform, circle.Color, circle.Thickness, circle.Fade, (int)e);
					}
				}
			}

			// Draw Text
			{
				auto view = m_Registry.view<TransformComponent, TextComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					if (IsEnabled(entity))
					{
						auto [transform, text] = view.get<TransformComponent, TextComponent>(e);
						Vesuvius::DrawString(text.TextString, transform.GlobalTransform, text, (int)e);
					}
				}
			}

			Vesuvius::EndScene();
		}
		else if (!m_IsPaused)
		{
			std::string error = "[Scene::OnUpdateRuntime] No primary camera enabled!";
			RuntimeErrorHit(error);
		}
	}

	void Scene::UpdateTransformRecursive(Entity entity)
	{
		auto& transformComponent = entity.GetComponent<TransformComponent>();

		// Build local transform from vec3 rotation
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), transformComponent.Translation);
		glm::mat4 rotation =
			glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.z, glm::vec3(0, 0, 1)) *
			glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.y, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), transformComponent.Scale);

		glm::mat4 localTransform = translation * rotation * scale;

		UUID parentUUID = GetParent(entity.GetUUID());
		if (parentUUID != 0)
		{
			auto& parentTransform = GetEntityByUUID(parentUUID).GetComponent<TransformComponent>();
			transformComponent.GlobalTransform = parentTransform.GlobalTransform * localTransform;
		}
		else
		{
			transformComponent.GlobalTransform = localTransform;
		}

		// Recursively update children
		const auto& children = GetChildren(entity.GetUUID());
		for (UUID childUUID : children)
		{
			Entity child = GetEntityByUUID(childUUID);
			UpdateTransformRecursive(child);
		}
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = m_Registry.view<TagComponent, Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& id = entity.GetComponent<IDComponent>();
					if (id.Enabled)
					{
						auto& transform = entity.GetComponent<TransformComponent>();
						auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

						b2Body* body = (b2Body*)rb2d.RuntimeBody;
						const auto& position = body->GetPosition();
						transform.Translation.x = position.x;
						transform.Translation.y = position.y;
						transform.Rotation.z = body->GetAngle();
					}
				}
			}
		}

		// Render
		RenderScene(camera);
	}

	bool Scene::IsEnabled(Entity entity)
	{
		if (!entity.GetComponent<IDComponent>().Enabled)
		{
			return false;
		}
		else if (GetParent(entity.GetUUID()) != 0)
		{
			UUID id = entity.GetUUID();
			UUID parentID = GetParent(id);
			Entity parent = GetEntityByUUID(parentID);
			return IsEnabled(parent);
		}
		else
		{
			return true;
		}
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		Vesuvius::BeginScene(camera);

		for (auto entity : m_Registry.view<TransformComponent>())
		{
			Entity e = { entity, this };
			if (GetParent(e.GetUUID()) == 0)
				UpdateTransformRecursive(e); // now truly recursive
		}

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				Entity entity = { e, this };
				if (IsEnabled(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
					Vesuvius::DrawSprite(transform.GlobalTransform, sprite, (int)e);
				}
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				if (IsEnabled(entity))
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(e);
					Vesuvius::DrawCircle(transform.GlobalTransform, circle.Color, circle.Thickness, circle.Fade, (int)e);
				}
			}
		}

		// Draw text
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& id = entity.GetComponent<IDComponent>();
				if (IsEnabled(entity))
				{
					auto [transform, text] = view.get<TransformComponent, TextComponent>(e);
					Vesuvius::DrawString(text.TextString, transform.GlobalTransform, text, (int)e);
				}
			}
		}

		Vesuvius::EndScene();
	}

	void Scene::RenderScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		Vesuvius::BeginScene(camera, cameraTransform);

		for (auto entity : m_Registry.view<TransformComponent>())
		{
			Entity e = { entity, this };
			if (GetParent(e.GetUUID()) == 0)
				UpdateTransformRecursive(e); // now truly recursive
		}

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				Entity entity = { e, this };
				if (IsEnabled(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
					Vesuvius::DrawSprite(transform.GlobalTransform, sprite, (int)e);
				}
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				if (IsEnabled(entity))
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(e);
					Vesuvius::DrawCircle(transform.GlobalTransform, circle.Color, circle.Thickness, circle.Fade, (int)e);
				}
			}
		}

		// Draw text
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& id = entity.GetComponent<IDComponent>();
				if (IsEnabled(entity))
				{
					auto [transform, text] = view.get<TransformComponent, TextComponent>(e);
					Vesuvius::DrawString(text.TextString, transform.GlobalTransform, text, (int)e);
				}
			}
		}

		Vesuvius::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::OnPhysics2DStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		m_ContactListener = std::make_unique<ContactListener2D>();
		m_PhysicsWorld->SetContactListener(m_ContactListener.get());

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			//
			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(transform.GlobalTransform, translation, rotation, scale);
			//

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(translation.x, translation.y);
			bodyDef.angle = rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entity.GetComponent<IDComponent>().ID);			
			body->SetFixedRotation(rb2d.FixedRotation);
			body->SetGravityScale(rb2d.GravityScale);

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center = body->GetLocalCenter();
			massData.I = 1.0f;

			body->SetMassData(&massData);

			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				glm::mat4 globalTransform = entity.GetComponent<TransformComponent>().GlobalTransform;

				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(globalTransform, translation, rotation, scale);

				glm::vec2 absScale = glm::abs(glm::vec2(scale.x, scale.y));

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * absScale.x, bc2d.Size.y * absScale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				fixtureDef.isSensor = bc2d.IsTrigger;

				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				fixtureDef.isSensor = cc2d.IsTrigger;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	glm::mat4 Scene::GetWorldTransform(UUID entityID)
	{
		Entity entity = GetEntityByUUID(entityID);
		const auto& transform = entity.GetComponent<TransformComponent>();

		glm::mat4 local = glm::translate(glm::mat4(1.0f), transform.Translation)
			* glm::toMat4(glm::quat(transform.Rotation)) 
			* glm::scale(glm::mat4(1.0f), transform.Scale);

		if (GetParent(entityID) == 0)
			return local;

		glm::mat4 parentTransform = GetWorldTransform(GetParent(entityID));
		return parentTransform * local;
	}

	glm::mat4 Scene::ComposeTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
			* glm::toMat4(glm::quat(rotation))
			* glm::scale(glm::mat4(1.0f), scale);

		return transform;
	}

	void Scene::OnRenderRuntime()
	{
		Entity cameraEntity = GetPrimaryCameraEntity();
		if (!cameraEntity)
			return;

		auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
		auto& cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();

		Vesuvius::BeginScene(camera, cameraTransform);

		for (auto entity : m_Registry.view<TransformComponent>())
		{
			Entity e = { entity, this };
			if (GetParent(e.GetUUID()) == 0)
				UpdateTransformRecursive(e); // now truly recursive
		}

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto e : group)
			{
				Entity entity = { e, this };
				if (IsEnabled(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
					Vesuvius::DrawSprite(transform.GlobalTransform, sprite, (int)e);
				}
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				if (IsEnabled(entity))
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(e);
					Vesuvius::DrawCircle(transform.GlobalTransform, circle.Color, circle.Thickness, circle.Fade, (int)e);
				}
			}
		}

		// Draw text
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& id = entity.GetComponent<IDComponent>();
				if (IsEnabled(entity))
				{
					auto [transform, text] = view.get<TransformComponent, TextComponent>(e);
					Vesuvius::DrawString(text.TextString, transform.GlobalTransform, text, (int)e);
				}
			}
		}

		Vesuvius::EndScene();
	}

	void Scene::SetWorldTransform(UUID entityID, const glm::mat4& worldTransform)
	{
		Entity entity = GetEntityByUUID(entityID);
		if (!entity)
			return;

		auto& transform = entity.GetComponent<TransformComponent>();

		glm::mat4 parentTransform = glm::mat4(1.0f);
		UUID parentID = GetParent(entityID);
		if (parentID)
			parentTransform = GetWorldTransform(parentID);

		// Prevent invalid matrix issues
		if (glm::determinant(parentTransform) == 0.0f)
			return;

		glm::mat4 localTransform = glm::inverse(parentTransform) * worldTransform;

		glm::vec3 translation, rotation, scale;
		if (Math::DecomposeTransform(localTransform, translation, rotation, scale))
		{
			transform.Translation = translation;
			transform.Rotation = rotation;
			transform.Scale = scale;
		}
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		newEntity.GetComponent<IDComponent>().Enabled = entity.GetComponent<IDComponent>().Enabled;
		CopyComponentIfExists(AllComponents{}, newEntity, entity);

		if (GetParent(entity.GetComponent<IDComponent>().ID) != 0)
		{
			SetParent(newEntity.GetComponent<IDComponent>().ID, GetParent(entity.GetComponent<IDComponent>().ID));
		}

		// Recursively duplicate children
		auto [begin, end] = m_EntityChildrenMap.equal_range(entity.GetComponent<IDComponent>().ID);
		for (auto it = begin; it != end; ++it)
		{
			UUID childID = it->second;
			Entity childEntity = GetEntityByUUID(childID);

			if (!childEntity || !childEntity.HasComponent<IDComponent>())
				continue;

			Entity duplicatedChild = DuplicateEntity(childEntity); // Recursively duplicate

			// Set new parent to duplicated entity
			if (GetParent(entity.GetComponent<IDComponent>().ID) != 0)
				SetParent(duplicatedChild.GetComponent<IDComponent>().ID, newEntity.GetComponent<IDComponent>().ID);
		}

		return newEntity;
	}

	void Scene::SetParent(UUID entity, UUID parent)
	{
		if (entity == parent)
		{
			std::string error = "[Scene::SetParent] Entity cannot be its own parent.";
			RuntimeErrorHit(error);
			return;
		}

		UUID current = parent;
		while (current != 0)
		{
			if (current == entity)
			{
				std::string error = "[Scene::SetParent] Entity's parent cannot be one of its descendants.";
				RuntimeErrorHit(error);
				return;
			}
			current = GetParent(current); // Traverse up the hierarchy
		}

		glm::mat4 oldGlobalTransform = GetEntityByUUID(entity).GetComponent<TransformComponent>().GlobalTransform;

		RemoveParent(entity);
		m_EntityParentMap[entity] = parent;
		m_EntityChildrenMap.emplace(parent, entity);

		glm::mat4 newParentGlobalTransform = GetEntityByUUID(parent).GetComponent<TransformComponent>().GlobalTransform;
		glm::mat4 newLocalTransform = glm::inverse(newParentGlobalTransform) * oldGlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(newLocalTransform, translation, rotation, scale);

		TransformComponent& tc = GetEntityByUUID(entity).GetComponent<TransformComponent>();
		tc.Translation = translation;
		tc.Rotation = rotation;
		tc.Scale = scale;
	}

	void Scene::SetParentOnLoad(UUID entity, UUID parent)
	{
		if (entity == parent)
		{
			std::string error = "[Scene::SetParent] Entity cannot be its own parent.";
			RuntimeErrorHit(error);
			return;
		}

		UUID current = parent;
		while (current != 0)
		{
			if (current == entity)
			{
				std::string error = "[Scene::SetParent] Entity's parent cannot be one of its descendants.";
				RuntimeErrorHit(error);
				return;
			}
			current = GetParent(current); // Traverse up the hierarchy
		}

		RemoveParent(entity);
		m_EntityParentMap[entity] = parent;
		m_EntityChildrenMap.emplace(parent, entity);
	}

	void Scene::RemoveParent(UUID entity)
	{
		auto it = m_EntityParentMap.find(entity);
		if (it != m_EntityParentMap.end())
		{
			// 1. Cache the current global transform
			glm::mat4 globalTransform = GetEntityByUUID(entity).GetComponent<TransformComponent>().GlobalTransform;

			// 2. Remove from parent's child list
			UUID oldParent = it->second;
			m_EntityParentMap.erase(it);

			auto range = m_EntityChildrenMap.equal_range(oldParent);
			for (auto i = range.first; i != range.second; ++i)
			{
				if (i->second == entity)
				{
					m_EntityChildrenMap.erase(i);
					break;
				}
			}

			// 3. Since the entity is now root-level, set local transform = global transform
			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(globalTransform, translation, rotation, scale);

			TransformComponent& tc = GetEntityByUUID(entity).GetComponent<TransformComponent>();
			tc.Translation = translation;
			tc.Rotation = rotation;
			tc.Scale = scale;
		}
	}

	UUID Scene::GetParent(UUID entity)
	{
		auto it = m_EntityParentMap.find(entity);
		return it != m_EntityParentMap.end() ? it->second : 0;
	}

	std::vector<UUID> Scene::GetChildren(UUID entity)
	{
		std::vector<UUID> result;
		auto range = m_EntityChildrenMap.equal_range(entity);
		for (auto it = range.first; it != range.second; ++it)
			result.push_back(it->second);
		return result;
	}

	void Scene::RuntimeErrorHit(std::string& error)
	{
		CC_CORE_ERROR(error);
		if (IsRunning())
			m_IsPaused = true;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<MonoScriptComponent>(Entity entity, MonoScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TextComponent>(Entity entity, TextComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<AudioSourceComponent>(Entity entity, AudioSourceComponent& component)
	{
		if (!component.FilePath.empty())
		{
			component.AudioClip = AudioEngine::LoadClip(component.FilePath);
			if (component.AudioClip)
				component.AudioClip->SetLooping(component.Loop);
		}
	}
}
