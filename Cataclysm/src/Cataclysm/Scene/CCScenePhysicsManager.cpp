#include "ccpch.h"
#include "Cataclysm/Scene/CCScenePhysicsManager.h"

#include "Cataclysm/Scene/CCSceneEntityManager.h"
#include "Cataclysm/Physics/CCContactListener2D.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"

#include "Cataclysm/Math/CCMath.h"
#include "Cataclysm/Physics/CCPhysics2D.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Cataclysm
{
	void CCScenePhysicsManager::OnPhysics2DStart(CCScene* scene)
	{
		scene->SetPhysicsWorld(new b2World({ 0.0f, -9.8f }));
		scene->m_ContactListener = std::make_unique<CCContactListener2D>();
		scene->GetPhysicsWorld()->SetContactListener(scene->m_ContactListener.get());

		auto view = scene->GetRegistry().view<Rigidbody2D>();
		for (auto e : view)
		{
			Entity entity = { e, scene };
			auto& transform = entity.GetComponent<Transform>();
			auto& rb2d = entity.GetComponent<Rigidbody2D>();

			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(transform.GlobalTransform, translation, rotation, scale);

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(translation.x, translation.y);
			bodyDef.angle = rotation.z;

			b2Body* body = scene->GetPhysicsWorld()->CreateBody(&bodyDef);
			body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entity.GetComponent<IDComponent>().ID);
			body->SetFixedRotation(rb2d.FixedRotation);
			body->SetGravityScale(rb2d.GravityScale);

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center = body->GetLocalCenter();
			massData.I = 1.0f;

			body->SetMassData(&massData);

			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2D>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2D>();

				glm::mat4 globalTransform = entity.GetComponent<Transform>().GlobalTransform;

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

			if (entity.HasComponent<CircleCollider2D>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2D>();

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

	void CCScenePhysicsManager::OnPhysics2DStop(CCScene* scene)
	{
		scene->ClearPhysicsWorld();
	}

	void CCScenePhysicsManager::ProcessPhysics(CCScene* scene, float ts)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		scene->GetPhysicsWorld()->Step(ts, velocityIterations, positionIterations);

		auto view = scene->GetRegistry().view<Rigidbody2D>();
		for (auto e : view)
		{
			Entity entity = { e, scene };
			auto& id = entity.GetComponent<IDComponent>();
			if (!CCSceneEntityManager::IsEnabled(scene, entity)) continue;

			if (!entity.HasComponent<Transform>())
			{
				std::string error = "[CCScene::UpdateSceneRuntime] Entity '" + entity.GetName() + "' does not contain component <Transform>";
				scene->RuntimeErrorHit(error);
			}

			auto& tc = entity.GetComponent<Transform>();
			auto& rb2d = entity.GetComponent<Rigidbody2D>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;

			if (rb2d.Type == Rigidbody2D::BodyType::Dynamic)
			{
				const b2Vec2 position = body->GetPosition();
				const float angle = body->GetAngle();

				glm::vec3 physicsTranslation = { position.x, position.y, tc.Translation.z };
				glm::vec3 physicsRotation = { 0.0f, 0.0f, angle };

				glm::mat4 newGlobalTransform =
					glm::translate(glm::mat4(1.0f), physicsTranslation) *
					glm::rotate(glm::mat4(1.0f), physicsRotation.z, glm::vec3(0, 0, 1)) *
					glm::scale(glm::mat4(1.0f), tc.Scale); // Use original local scale

				CCSceneEntityManager::SetWorldTransform(scene, entity.GetUUID(), newGlobalTransform);
			}

		}

		CCSceneEntityManager::UpdateTransforms(scene);
	}

	void CCScenePhysicsManager::ProcessCollisions(CCScene* scene)
	{
		const auto& collisions = scene->m_ContactListener->GetCollisions();
		for (const auto& collision : collisions)
		{
			Entity entityA = CCSceneEntityManager::GetEntityByUUID(scene, collision.EntityA);
			Entity entityB = CCSceneEntityManager::GetEntityByUUID(scene, collision.EntityB);

			if (entityA.HasComponent<MonoScript>())
				CCScriptEngine::OnCollisionEnter2D(entityA, entityB);

			if (entityB.HasComponent<MonoScript>())
				CCScriptEngine::OnCollisionEnter2D(entityB, entityA);
		}

		const auto& endCollisions = scene->m_ContactListener->GetEndCollisions();
		for (const auto& endCollision : endCollisions)
		{
			Entity entityA = CCSceneEntityManager::GetEntityByUUID(scene, endCollision.EntityA);
			Entity entityB = CCSceneEntityManager::GetEntityByUUID(scene, endCollision.EntityB);

			if (entityA.HasComponent<MonoScript>())
				CCScriptEngine::OnCollisionExit2D(entityA, entityB);

			if (entityB.HasComponent<MonoScript>())
				CCScriptEngine::OnCollisionExit2D(entityB, entityA);
		}


		scene->m_ContactListener->Clear();
		scene->m_ContactListener->ClearEndCollisions();
	}
}
