#include "ccpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Cataclysm/Core/UUID.h"
#include "Cataclysm/Input/KeyCodes.h"
#include "Cataclysm/Input/Input.h"
#include "Cataclysm/Math/Math.h"

#include "Cataclysm/ECS/Entity.h"
#include "Cataclysm/Scene/Scene.h"

#include "Cataclysm/Physics/Physics2D.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"

namespace Cataclysm 
{

	namespace Utils 
	{
		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}
	}

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define CC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Cataclysm.InternalCalls::" #Name, Name)


	static MonoObject* GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
	}

	///////////////////////////////////////////
	////////////////// DEBUG //////////////////
	///////////////////////////////////////////

	static void Debug_Log(MonoString* message)
	{
		std::string str = Utils::MonoStringToString(message);
		CC_CORE_TRACE(str);
	}

	static void Debug_Info(MonoString* message)
	{
		std::string str = Utils::MonoStringToString(message);
		CC_CORE_INFO(str);
	}

	static void Debug_Warn(MonoString* message)
	{
		std::string str = Utils::MonoStringToString(message);
		CC_CORE_WARN(str);
	}

	static void Debug_Error(MonoString* message)
	{
		std::string str = Utils::MonoStringToString(message);

		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");

		scene->RuntimeErrorHit(str);
	}

	///////////////////////////////////////////
	////////////////// ENTITY /////////////////
	///////////////////////////////////////////

	static void Entity_InstantiateEmpty(MonoString* name, UUID* outEntityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->CreateEntity(Utils::MonoStringToString(name));
		*outEntityID = entity.GetUUID();
	}

	static void Entity_InstantiateEmptyAsChild(MonoString* name, UUID parentID, UUID* outEntityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->CreateEntity(Utils::MonoStringToString(name));
		scene->SetParent(entity.GetUUID(), parentID);
		*outEntityID = entity.GetUUID();
	}

	static void Entity_Destroy(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		scene->DestroyEntity(entity);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		if (!(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end()))
			return false;
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void Entity_AddBoxCollider2DComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<BoxCollider2DComponent>())
			entity.AddComponent<BoxCollider2DComponent>();
	}

	static void Entity_AddCameraComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<CameraComponent>())
			entity.AddComponent<CameraComponent>();
	}

	static void Entity_AddCircleCollider2DComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<CircleCollider2DComponent>())
			entity.AddComponent<CircleCollider2DComponent>();
	}

	static void Entity_AddCircleRendererComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<CircleRendererComponent>())
			entity.AddComponent<CircleRendererComponent>();
	}

	static void Entity_AddRigidbody2DComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<Rigidbody2DComponent>())
			entity.AddComponent<Rigidbody2DComponent>();
	}

	static void Entity_AddSpriteRendererComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<SpriteRendererComponent>())
			entity.AddComponent<SpriteRendererComponent>();
	}

	static void Entity_AddTextComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<TextComponent>())
			entity.AddComponent<TextComponent>();
	}

	static void Entity_RemoveBoxCollider2DComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<BoxCollider2DComponent>())
			entity.RemoveComponent<BoxCollider2DComponent>();
	}

	static void Entity_RemoveCameraComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<CameraComponent>())
			entity.RemoveComponent<CameraComponent>();
	}

	static void Entity_RemoveCircleCollider2DComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<CircleCollider2DComponent>())
			entity.RemoveComponent<CircleCollider2DComponent>();
	}

	static void Entity_RemoveCircleRendererComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<CircleRendererComponent>())
			entity.RemoveComponent<CircleRendererComponent>();
	}

	static void Entity_RemoveSpriteRendererComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<SpriteRendererComponent>())
			entity.RemoveComponent<SpriteRendererComponent>();
	}

	static void Entity_RemoveTextComponent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<TextComponent>())
			entity.RemoveComponent<TextComponent>();
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	static bool Entity_GetEnabled(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");
		return entity.GetComponent<IDComponent>().Enabled;
	}

	static void Entity_SetEnabled(UUID entityID, bool enabled)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		entity.GetComponent<IDComponent>().Enabled = enabled;
	}

	static void Entity_SetParent(UUID entityID, UUID parentID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		Entity parent = scene->GetEntityByUUID(parentID);
		CC_CORE_ASSERT(entity, "Parent does not exist!");

		scene->SetParent(entityID, parentID);
	}

	static void Entity_RemoveParent(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		scene->RemoveParent(entityID);
	}

	static void Entity_GetParent(UUID entityID, UUID* outParentID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outParentID = scene->GetParent(entityID);
	}

	//static void Entity_GetChildren(UUID entityID, std::vector<UUID>* outChildren)
	//{
	//	Scene* scene = ScriptEngine::GetSceneContext();
	//	CC_CORE_ASSERT(scene, "Scene does not exist!");
	//	Entity entity = scene->GetEntityByUUID(entityID);
	//	CC_CORE_ASSERT(entity, "Entity does not exist!");
	//
	//	*outChildren = scene->GetChildren(entityID);
	//}


	///////////////////////////////////////////
	////////////////// CAMERA /////////////////
	///////////////////////////////////////////

	static bool CameraComponent_GetPrimary(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Primary;
	}

	static void CameraComponent_SetPrimary(UUID entityID, bool primary)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Primary = primary;
	}

	static bool CameraComponent_GetFixedAspectRatio(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.FixedAspectRatio;
	}

	static void CameraComponent_SetFixedAspectRatio(UUID entityID, bool fixedAspectRatio)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.FixedAspectRatio = fixedAspectRatio;
	}

	static float CameraComponent_GetOrthographicSize(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Camera.GetOrthographicSize();
	}

	static void CameraComponent_SetOrthographicSize(UUID entityID, float size)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Camera.SetOrthographicSize(size);
	}

	static float CameraComponent_GetPerspectiveVerticalFOV(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Camera.GetPerspectiveVerticalFOV();
	}

	static void CameraComponent_SetPerspectiveVerticalFOV(UUID entityID, float fov)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Camera.SetPerspectiveVerticalFOV(fov);
	}

	static float CameraComponent_GetOrthographicNearClip(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Camera.GetOrthographicNearClip();
	}

	static void CameraComponent_SetOrthographicNearClip(UUID entityID, float clip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Camera.SetOrthographicNearClip(clip);
	}

	static float CameraComponent_GetOrthographicFarClip(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Camera.GetOrthographicFarClip();
	}

	static void CameraComponent_SetOrthographicFarClip(UUID entityID, float clip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Camera.SetOrthographicFarClip(clip);
	}

	static float CameraComponent_GetPerspectiveNearClip(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Camera.GetPerspectiveNearClip();
	}

	static void CameraComponent_SetPerspectiveNearClip(UUID entityID, float clip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Camera.SetPerspectiveNearClip(clip);
	}

	static float CameraComponent_GetPerspectiveFarClip(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		return cc.Camera.GetPerspectiveFarClip();
	}

	static void CameraComponent_SetPerspectiveFarClip(UUID entityID, float clip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<CameraComponent>();
		cc.Camera.SetPerspectiveFarClip(clip);
	}

	static void CameraComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CameraComponent>().Reset();
	}


	///////////////////////////////////////////
	//////////////// TRANSFORM ////////////////
	///////////////////////////////////////////

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID entityID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<TransformComponent>().Scale = *scale;
	}

	static void TransformComponent_GetGlobalTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		const glm::mat4& globalTransform = entity.GetComponent<TransformComponent>().GlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(globalTransform, translation, rotation, scale);

		*outTranslation = translation;
	}

	static void TransformComponent_GetGlobalRotation(UUID entityID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		const glm::mat4& globalTransform = entity.GetComponent<TransformComponent>().GlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(globalTransform, translation, rotation, scale);

		*outRotation = rotation;
	}

	static void TransformComponent_GetGlobalScale(UUID entityID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		const glm::mat4& globalTransform = entity.GetComponent<TransformComponent>().GlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(globalTransform, translation, rotation, scale);

		*outScale = scale;
	}

	static void TransformComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<TransformComponent>().Reset();
	}


	///////////////////////////////////////////
	/////////////// RIGIDBODY2D ///////////////
	///////////////////////////////////////////

	static void Rigidbody2DComponent_ApplyForce(UUID entityID, glm::vec2* force, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForce(b2Vec2(force->x, force->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyForceToCenter(UUID entityID, glm::vec2* force, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyForceToCenter(b2Vec2(force->x, force->y), wake);
	}

	static void Rigidbody2DComponent_ApplyTorque(UUID entityID, float torque, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyTorque(torque, wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2DComponent_ApplyAngularImpulse(UUID entityID, float impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyAngularImpulse(impulse, wake);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2* outLinearVelocity)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		const b2Vec2& linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static void Rigidbody2DComponent_SetLinearVelocity(UUID entityID, glm::vec2* velocity)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& component = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)component.RuntimeBody;
		body->SetLinearVelocity({ velocity->x, velocity->y });
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		return Utils::Rigidbody2DTypeFromBox2DBody(body->GetType());
	}

	static float Rigidbody2DComponent_GetGravityScale(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Rigidbody2DComponent>(), "Entity does not have TextComponent!");
		return entity.GetComponent<Rigidbody2DComponent>().GravityScale;
	}

	static void Rigidbody2DComponent_SetGravityScale(UUID entityID, float gravityScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetGravityScale(gravityScale);
		rb2d.GravityScale = gravityScale;
	}

	static float Rigidbody2DComponent_GetMass(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Rigidbody2DComponent>(), "Entity does not have TextComponent!");
		return entity.GetComponent<Rigidbody2DComponent>().Mass;
	}

	static void Rigidbody2DComponent_SetMass(UUID entityID, float mass)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		b2MassData massData;
		massData.mass = mass;

		body->SetMassData(&massData);
		rb2d.Mass = mass;
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, Rigidbody2DComponent::BodyType bodyType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetType(Utils::Rigidbody2DTypeToBox2DBody(bodyType));
	}

	static void Rigidbody2DComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Rigidbody2DComponent>().Reset();
	}


	///////////////////////////////////////////
	////////////////// TEXT ///////////////////
	///////////////////////////////////////////

	static MonoString* TextComponent_GetText(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		return ScriptEngine::CreateString(tc.TextString.c_str());
	}

	static void TextComponent_SetText(UUID entityID, MonoString* textString)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		tc.TextString = Utils::MonoStringToString(textString);
	}

	static void TextComponent_GetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		*color = tc.Color;
	}

	static void TextComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		tc.Color = *color;
	}

	static float TextComponent_GetKerning(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		return tc.Kerning;
	}

	static void TextComponent_SetKerning(UUID entityID, float kerning)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		tc.Kerning = kerning;
	}

	static float TextComponent_GetLineSpacing(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		return tc.LineSpacing;
	}

	static void TextComponent_SetLineSpacing(UUID entityID, float lineSpacing)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<TextComponent>(), "Entity does not have TextComponent!");
		auto& tc = entity.GetComponent<TextComponent>();
		tc.LineSpacing = lineSpacing;
	}

	static void TextComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<TextComponent>().Reset();
	}


	///////////////////////////////////////////
	///////////// SPRITERENDERER //////////////
	///////////////////////////////////////////

	static void SpriteRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static float SpriteRendererComponent_GetTilingFactor(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<SpriteRendererComponent>(), "Entity does not have SpriteRendererComponent!");
		auto& src = entity.GetComponent<SpriteRendererComponent>();
		return src.TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(UUID entityID, float tilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<SpriteRendererComponent>(), "Entity does not have SpriteRendererComponent!");
		auto& src = entity.GetComponent<SpriteRendererComponent>();
		src.TilingFactor = tilingFactor;
	}

	static void SpriteRendererComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<SpriteRendererComponent>().Reset();
	}


	///////////////////////////////////////////
	///////////// CIRCLERENDERER //////////////
	///////////////////////////////////////////

	static void CircleRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CircleRendererComponent>().Color = *color;
	}

	static float CircleRendererComponent_GetThickness(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRendererComponent>(), "Entity does not have CircleRendererComponent!");
		auto& crc = entity.GetComponent<CircleRendererComponent>();
		return crc.Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID entityID, float thickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRendererComponent>(), "Entity does not have CircleRendererComponent!");
		auto& crc = entity.GetComponent<CircleRendererComponent>();
		crc.Thickness = thickness;
	}

	static float CircleRendererComponent_GetFade(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRendererComponent>(), "Entity does not have CircleRendererComponent!");
		auto& crc = entity.GetComponent<CircleRendererComponent>();
		return crc.Fade;
	}

	static void CircleRendererComponent_SetFade(UUID entityID, float fade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRendererComponent>(), "Entity does not have CircleRendererComponent!");
		auto& crc = entity.GetComponent<CircleRendererComponent>();
		crc.Fade = fade;
	}

	static void CircleRendererComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CircleRendererComponent>().Reset();
	}


	///////////////////////////////////////////
	////////////////// INPUT //////////////////
	///////////////////////////////////////////

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyDown(keycode);
	}

	static bool Input_IsMouseButtonDown(MouseCode button)
	{
		return Input::IsMouseButtonDown(button);
	}

	static void Input_GetMousePosition(glm::vec2* outPosition)
	{
		*outPosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
	}


	///////////////////////////////////////////
	////////////// BOXCOLLIDER2D //////////////
	///////////////////////////////////////////

	static void BoxCollider2DComponent_GetOffset(UUID entityID, glm::vec2* offset)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		*offset = bc2d.Offset;
	}

	static void BoxCollider2DComponent_SetOffset(UUID entityID, glm::vec2* offset)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.Offset = *offset;
	}

	static void BoxCollider2DComponent_GetSize(UUID entityID, glm::vec2* size)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		*size = bc2d.Size;
	}

	static void BoxCollider2DComponent_SetSize(UUID entityID, glm::vec2* size)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.Size = *size;
	}

	static float BoxCollider2DComponent_GetDensity(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		return bc2d.Density;
	}

	static void BoxCollider2DComponent_SetDensity(UUID entityID, float density)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.Density = density;
	}

	static float BoxCollider2DComponent_GetFriction(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		return bc2d.Friction;
	}

	static void BoxCollider2DComponent_SetFriction(UUID entityID, float friction)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.Friction = friction;
	}

	static float BoxCollider2DComponent_GetRestitution(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		return bc2d.Restitution;
	}

	static void BoxCollider2DComponent_SetRestitution(UUID entityID, float restitution)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.Restitution = restitution;
	}

	static float BoxCollider2DComponent_GetRestitutionThreshold(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		return bc2d.RestitutionThreshold;
	}

	static void BoxCollider2DComponent_SetRestitutionThreshold(UUID entityID, float restitutionThreshold)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.RestitutionThreshold = restitutionThreshold;
	}

	static bool BoxCollider2DComponent_GetIsTrigger(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		return bc2d.IsTrigger;
	}

	static void BoxCollider2DComponent_SetIsTrigger(UUID entityID, bool isTrigger)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity does not have BoxCollider2DComponent!");
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		bc2d.IsTrigger = isTrigger;
	}

	static void BoxCollider2DComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<BoxCollider2DComponent>().Reset();
	}


	///////////////////////////////////////////
	//////////// CIRCLECOLLIDER2D /////////////
	///////////////////////////////////////////

	static void CircleCollider2DComponent_GetOffset(UUID entityID, glm::vec2* offset)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		*offset = cc2d.Offset;
	}

	static void CircleCollider2DComponent_SetOffset(UUID entityID, glm::vec2* offset)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.Offset = *offset;
	}

	static float CircleCollider2DComponent_GetRadius(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		return cc2d.Radius;
	}

	static void CircleCollider2DComponent_SetRadius(UUID entityID, float radius)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.Radius = radius;
	}

	static float CircleCollider2DComponent_GetDensity(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		return cc2d.Density;
	}

	static void CircleCollider2DComponent_SetDensity(UUID entityID, float density)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.Density = density;
	}

	static float CircleCollider2DComponent_GetFriction(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		return cc2d.Friction;
	}

	static void CircleCollider2DComponent_SetFriction(UUID entityID, float friction)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.Friction = friction;
	}

	static float CircleCollider2DComponent_GetRestitution(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		return cc2d.Restitution;
	}

	static void CircleCollider2DComponent_SetRestitution(UUID entityID, float restitution)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.Restitution = restitution;
	}

	static float CircleCollider2DComponent_GetRestitutionThreshold(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		return cc2d.RestitutionThreshold;
	}

	static void CircleCollider2DComponent_SetRestitutionThreshold(UUID entityID, float restitutionThreshold)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.RestitutionThreshold = restitutionThreshold;
	}

	static bool CircleCollider2DComponent_GetIsTrigger(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		return cc2d.IsTrigger;
	}

	static void CircleCollider2DComponent_SetIsTrigger(UUID entityID, bool isTrigger)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2DComponent>(), "Entity does not have CircleCollider2DComponent!");
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		cc2d.IsTrigger = isTrigger;
	}

	static void CircleCollider2DComponent_Reset(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = scene->GetEntityByUUID(entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CircleCollider2DComponent>().Reset();
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("Cataclysm.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				CC_CORE_ERROR("[ScriptGlue::RegisterComponent] Could not find component type '{}'", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		CC_ADD_INTERNAL_CALL(Debug_Log);
		CC_ADD_INTERNAL_CALL(Debug_Info);
		CC_ADD_INTERNAL_CALL(Debug_Warn);
		CC_ADD_INTERNAL_CALL(Debug_Error);

		CC_ADD_INTERNAL_CALL(GetScriptInstance);

		CC_ADD_INTERNAL_CALL(Entity_InstantiateEmpty);
		CC_ADD_INTERNAL_CALL(Entity_InstantiateEmptyAsChild);
		CC_ADD_INTERNAL_CALL(Entity_Destroy);

		CC_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		CC_ADD_INTERNAL_CALL(Entity_HasComponent);

		CC_ADD_INTERNAL_CALL(Entity_GetEnabled);
		CC_ADD_INTERNAL_CALL(Entity_SetEnabled);

		CC_ADD_INTERNAL_CALL(Entity_SetParent);
		CC_ADD_INTERNAL_CALL(Entity_GetParent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveParent);

		CC_ADD_INTERNAL_CALL(Entity_AddBoxCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCameraComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCircleCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCircleRendererComponent);		
		CC_ADD_INTERNAL_CALL(Entity_AddRigidbody2DComponent);		
		CC_ADD_INTERNAL_CALL(Entity_AddSpriteRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddTextComponent);

		CC_ADD_INTERNAL_CALL(Entity_RemoveBoxCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveCameraComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveCircleCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveCircleRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveSpriteRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveTextComponent);

		//CC_ADD_INTERNAL_CALL(Entity_GetChildren);

		CC_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		CC_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		CC_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		CC_ADD_INTERNAL_CALL(TransformComponent_SetScale);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetGlobalTranslation);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetGlobalRotation);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetGlobalScale);
		CC_ADD_INTERNAL_CALL(TransformComponent_Reset);

		CC_ADD_INTERNAL_CALL(CameraComponent_GetPrimary);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetPrimary);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetFixedAspectRatio);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetFixedAspectRatio);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicSize);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicSize);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicNearClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicNearClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicFarClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicFarClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveVerticalFOV);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveVerticalFOV);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveNearClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveNearClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveFarClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveFarClip);
		CC_ADD_INTERNAL_CALL(CameraComponent_Reset);

		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyAngularImpulse);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyForce);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyForceToCenter);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyTorque);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetLinearVelocity);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetGravityScale);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetGravityScale);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetMass);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetMass);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_Reset);

		CC_ADD_INTERNAL_CALL(TextComponent_GetText);
		CC_ADD_INTERNAL_CALL(TextComponent_SetText);
		CC_ADD_INTERNAL_CALL(TextComponent_GetColor);
		CC_ADD_INTERNAL_CALL(TextComponent_SetColor);
		CC_ADD_INTERNAL_CALL(TextComponent_GetKerning);
		CC_ADD_INTERNAL_CALL(TextComponent_SetKerning);
		CC_ADD_INTERNAL_CALL(TextComponent_GetLineSpacing);
		CC_ADD_INTERNAL_CALL(TextComponent_SetLineSpacing);
		CC_ADD_INTERNAL_CALL(TextComponent_Reset);

		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);		
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_Reset);

		CC_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_Reset);

		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetOffset);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetOffset);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetSize);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetSize);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetDensity);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetDensity);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetFriction);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetFriction);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitution);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitution);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitutionThreshold);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitutionThreshold);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetIsTrigger);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetIsTrigger);
		CC_ADD_INTERNAL_CALL(BoxCollider2DComponent_Reset);

		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetOffset);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetOffset);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRadius);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRadius);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetDensity);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetDensity);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetFriction);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetFriction);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRestitution);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRestitution);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRestitutionThreshold);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRestitutionThreshold);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetIsTrigger);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetIsTrigger);
		CC_ADD_INTERNAL_CALL(CircleCollider2DComponent_Reset);
		
		CC_ADD_INTERNAL_CALL(Input_IsKeyDown);
		CC_ADD_INTERNAL_CALL(Input_IsMouseButtonDown);
		CC_ADD_INTERNAL_CALL(Input_GetMousePosition);
	}
}
