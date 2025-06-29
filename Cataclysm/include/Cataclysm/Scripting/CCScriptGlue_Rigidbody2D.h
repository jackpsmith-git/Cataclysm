#pragma once

#include "CCScriptEngine.h"

#include "Cataclysm/Physics/CCPhysics2D.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

#include "box2d/b2_body.h"

namespace Cataclysm
{
	static void Rigidbody2DComponent_ApplyForce(CCUUID entityID, glm::vec2* force, glm::vec2* point, bool wake)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForce(b2Vec2(force->x, force->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyForceToCenter(CCUUID entityID, glm::vec2* force, bool wake)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForceToCenter(b2Vec2(force->x, force->y), wake);
	}

	static void Rigidbody2DComponent_ApplyTorque(CCUUID entityID, float torque, bool wake)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyTorque(torque, wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(CCUUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(CCUUID entityID, glm::vec2* impulse, bool wake)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2DComponent_ApplyAngularImpulse(CCUUID entityID, float impulse, bool wake)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyAngularImpulse(impulse, wake);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(CCUUID entityID, glm::vec2* outLinearVelocity)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		const b2Vec2& linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static void Rigidbody2DComponent_SetLinearVelocity(CCUUID entityID, glm::vec2* velocity)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& component = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)component.RuntimeBody;
		body->SetLinearVelocity({ velocity->x, velocity->y });
	}

	static Rigidbody2D::BodyType Rigidbody2DComponent_GetType(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		return Utils::Rigidbody2DTypeFromBox2DBody(body->GetType());
	}

	static float Rigidbody2DComponent_GetGravityScale(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Rigidbody2D>(), "Entity does not have Text!");
		return entity.GetComponent<Rigidbody2D>().GravityScale;
	}

	static void Rigidbody2DComponent_SetGravityScale(CCUUID entityID, float gravityScale)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetGravityScale(gravityScale);
		rb2d.GravityScale = gravityScale;
	}

	static float Rigidbody2DComponent_GetMass(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Rigidbody2D>(), "Entity does not have Text!");
		return entity.GetComponent<Rigidbody2D>().Mass;
	}

	static void Rigidbody2DComponent_SetMass(CCUUID entityID, float mass)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		b2MassData massData;
		massData.mass = mass;

		body->SetMassData(&massData);
		rb2d.Mass = mass;
	}

	static void Rigidbody2DComponent_SetType(CCUUID entityID, Rigidbody2D::BodyType bodyType)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2D>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetType(Utils::Rigidbody2DTypeToBox2DBody(bodyType));
	}

	static void Rigidbody2DComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Rigidbody2D>().Reset();
	}
}
