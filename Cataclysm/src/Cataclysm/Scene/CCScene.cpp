#include "ccpch.h"
#include "Cataclysm/Scene/CCScene.h"

// Cataclysm
#include "Cataclysm/ECS/CCComponents.h"
#include "Cataclysm/ECS/CCEntity.h"
#include "Cataclysm/Math/CCMath.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/Vesuvius/VSRenderer2D.h"
#include "Cataclysm/Physics/CCPhysics2D.h"
#include "Cataclysm/Physics/CCContactListener2D.h"
#include "Cataclysm/Audio/CCAudioEngine.h"

#include "Cataclysm/Scene/CCSceneEntityManager.h"
#include "Cataclysm/Scene/CCScenePhysicsManager.h"
#include "Cataclysm/Scene/CCSceneScriptManager.h"
#include "Cataclysm/Scene/CCSceneAudioManager.h"

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
	CCScene::CCScene() {}
	CCScene::~CCScene() { delete m_PhysicsWorld; }

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<CCUUID, entt::entity>& enttMap)
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
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<CCUUID, entt::entity>& enttMap)
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

	CCRef<CCScene> CCScene::Copy(CCRef<CCScene> other)
	{
		CCRef<CCScene> newScene = CreateRef<CCScene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;
		newScene->SkyboxColor = other->SkyboxColor;
		newScene->Name = other->Name;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<CCUUID, entt::entity> enttMap;

		auto idView = srcSceneRegistry.view<IDComponent>();
		std::vector<entt::entity> entities(idView.begin(), idView.end());

		for (auto it = entities.rbegin(); it != entities.rend(); ++it)
		{
			entt::entity e = *it;
			CCUUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = CCSceneEntityManager::CreateEntityWithUUID(newScene.get(), uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
			newEntity.GetComponent<IDComponent>().Enabled = srcSceneRegistry.get<IDComponent>(e).Enabled;
		}

		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		auto& srcParentMap = other->GetEntityParentMap();
		auto& dstParentMap = newScene->GetEntityParentMap();

		for (const auto& [childUUID, parentUUID] : srcParentMap)
		{
			entt::entity newChild = enttMap[childUUID];
			entt::entity newParent = enttMap[parentUUID];

			CCSceneEntityManager::SetParent(newScene.get(), childUUID, parentUUID);
		}

		return newScene;
	}

	void CCScene::OnRuntimeStart()
	{
		m_IsRunning = true;

		CCScenePhysicsManager::OnPhysics2DStart(this);
		CCSceneScriptManager::OnScriptingStart(this);
	}

	void CCScene::OnRuntimeStop()
	{
		m_IsRunning = false;

		CCScenePhysicsManager::OnPhysics2DStop(this);
		CCSceneScriptManager::OnScriptingStop(this);
		CCSceneAudioManager::OnAudioStop(this);
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

	void CCScene::UpdateSceneRuntime(CCTimestep ts, Vesuvius::VSEditorCamera& camera)
	{
		CC_PROFILE_SCOPE("Update");

		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			CCSceneEntityManager::UpdateTransforms(this);
			CCSceneAudioManager::ProcessAudio(this);
			CCSceneScriptManager::ProcessScripts(this, ts);
			CCScenePhysicsManager::ProcessPhysics(this, ts);
			CCScenePhysicsManager::ProcessCollisions(this);
		}

		RenderScene(camera);
	}

	void CCScene::UpdateSceneEditor(Vesuvius::VSEditorCamera& camera)
	{
		RenderScene(camera);
	}

	void CCScene::UpdateGameRuntime()
	{
		RenderGame();
	}

	void CCScene::RenderScene(Vesuvius::VSEditorCamera& camera)
	{
		Vesuvius::VSRenderer2D::BeginScene(camera);
		CCSceneEntityManager::UpdateTransforms(this);
		RenderElements();
		Vesuvius::VSRenderer2D::EndScene();
	}

	void CCScene::RenderGame()
	{
		Entity cameraEntity = CCSceneEntityManager::GetPrimaryCameraEntity(this);

		auto& camera = cameraEntity.GetComponent<Camera>().SceneCamera;
		auto& cameraTransform = cameraEntity.GetComponent<Transform>().GetTransform();

		Vesuvius::VSRenderer2D::BeginScene(camera, cameraTransform);
		CCSceneEntityManager::UpdateTransforms(this);
		RenderElements();
		Vesuvius::VSRenderer2D::EndScene();
	}

	void CCScene::RenderElements()
	{
		RenderSprites();
		RenderCircles();
		RenderText();
	}

	void CCScene::RenderSprites()
	{
		auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);
		for (auto e : group)
		{
			Entity entity = { e, this };
			if (CCSceneEntityManager::IsEnabled(this, entity))
			{
				auto [transform, sprite] = group.get<Transform, SpriteRenderer>(e);

				glm::mat4 worldTransform = glm::mat4(1.0f);
				Entity current = entity;
				while (true)
				{
					auto& tf = current.GetComponent<Transform>();
					glm::mat4 localMat = CCSceneEntityManager::ComposeTransform(this, tf.Translation, tf.Rotation, tf.Scale);
					worldTransform = localMat * worldTransform;

					CCUUID parent = CCSceneEntityManager::GetParent(this, current.GetUUID());
					if (parent == 0)
						break;

					current = CCSceneEntityManager::GetEntityByUUID(this, parent);
				}

				if (sprite.Texture)
				{
					float textureWidth = (float)sprite.Texture->GetWidth();
					float textureHeight = (float)sprite.Texture->GetHeight();
					float PPU = CCProject::GetActive()->GetConfig().PixelDensity;

					glm::mat4 ppuScale = glm::scale(glm::mat4(1.0f), glm::vec3(
						textureWidth / PPU,
						textureHeight / PPU,
						1.0f
					));

					worldTransform *= ppuScale;
				}

				Vesuvius::VSRenderer2D::DrawSprite(worldTransform, sprite, (int)e);
			}
		}
	}

	void CCScene::RenderCircles()
	{
		auto view = m_Registry.view<Transform, CircleRenderer>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			if (CCSceneEntityManager::IsEnabled(this, entity))
			{
				auto [transform, circle] = view.get<Transform, CircleRenderer>(e);
				Vesuvius::VSRenderer2D::DrawCircle(transform.GlobalTransform, circle.Color, circle.Thickness, circle.Fade, (int)e);
			}
		}
	}

	void CCScene::RenderText()
	{
		auto view = m_Registry.view<Transform, Text>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& id = entity.GetComponent<IDComponent>();
			if (CCSceneEntityManager::IsEnabled(this, entity))
			{
				auto [transform, text] = view.get<Transform, Text>(e);
				Vesuvius::VSRenderer2D::DrawString(text.TextString, transform.GlobalTransform, text, (int)e);
			}
		}
	}

	void CCScene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<Camera>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<Camera>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.SceneCamera.SetViewportSize(width, height);
		}
	}

	void CCScene::RuntimeErrorHit(std::string& error)
	{
		CC_CORE_ERROR(error);
		if (IsRunning())
			m_IsPaused = true;
	}
}
