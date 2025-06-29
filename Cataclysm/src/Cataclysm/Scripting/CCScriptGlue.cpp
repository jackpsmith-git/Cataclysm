#include "ccpch.h"
#include "Cataclysm/Scripting/CCScriptGlue.h"

#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/Scripting/CCScriptUtils.h"

#include "Cataclysm/Scripting/CCScriptGlue_Entity.h"
#include "Cataclysm/Scripting/CCScriptGlue_Debug.h"
#include "Cataclysm/Scripting/CCScriptGlue_AudioSource.h"
#include "Cataclysm/Scripting/CCScriptGlue_Camera.h"
#include "Cataclysm/Scripting/CCScriptGlue_Transform.h"
#include "Cataclysm/Scripting/CCScriptGlue_Rigidbody2D.h"
#include "Cataclysm/Scripting/CCScriptGlue_Text.h"
#include "Cataclysm/Scripting/CCScriptGlue_SpriteRenderer.h"
#include "Cataclysm/Scripting/CCScriptGlue_CircleRenderer.h"
#include "Cataclysm/Scripting/CCScriptGlue_Input.h"
#include "Cataclysm/Scripting/CCScriptGlue_BoxCollider2D.h"
#include "Cataclysm/Scripting/CCScriptGlue_CircleCollider2D.h"

#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm 
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define CC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Cataclysm.InternalCalls::" #Name, Name)


	static MonoObject* GetScriptInstance(CCUUID entityID)
	{
		return CCScriptEngine::GetManagedInstance(entityID);
	}

	static bool Entity_HasComponent(CCUUID entityID, MonoReflectionType* componentType)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		if (!(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end()))
			return false;
		return s_EntityHasComponentFuncs.at(managedType)(entity);
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

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), CCScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				CC_CORE_ERROR("[CCScriptGlue::RegisterComponent] Could not find component type '{}'", managedTypename);
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

	void CCScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void CCScriptGlue::RegisterFunctions()
	{
		CC_ADD_INTERNAL_CALL(Debug_Log);
		CC_ADD_INTERNAL_CALL(Debug_Info);
		CC_ADD_INTERNAL_CALL(Debug_Warn);
		CC_ADD_INTERNAL_CALL(Debug_Error);

		CC_ADD_INTERNAL_CALL(GetScriptInstance);

		CC_ADD_INTERNAL_CALL(Entity_InstantiateEmpty);
		CC_ADD_INTERNAL_CALL(Entity_InstantiateEmptyAsChild);
		CC_ADD_INTERNAL_CALL(Entity_InstantiateEmptyWithoutName);
		CC_ADD_INTERNAL_CALL(Entity_Destroy);

		CC_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		CC_ADD_INTERNAL_CALL(Entity_HasComponent);

		CC_ADD_INTERNAL_CALL(Entity_GetEnabled);
		CC_ADD_INTERNAL_CALL(Entity_SetEnabled);

		CC_ADD_INTERNAL_CALL(Entity_SetParent);
		CC_ADD_INTERNAL_CALL(Entity_GetParent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveParent);

		CC_ADD_INTERNAL_CALL(Entity_AddAudioSourceComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddBoxCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCameraComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCircleCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCircleRendererComponent);		
		CC_ADD_INTERNAL_CALL(Entity_AddRigidbody2DComponent);		
		CC_ADD_INTERNAL_CALL(Entity_AddSpriteRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddTextComponent);

		CC_ADD_INTERNAL_CALL(Entity_RemoveAudioSourceComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveBoxCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveCameraComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveCircleCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveCircleRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveSpriteRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_RemoveTextComponent);

		//CC_ADD_INTERNAL_CALL(Entity_GetChildren);

		CC_ADD_INTERNAL_CALL(AudioSourceComponent_Play);
		CC_ADD_INTERNAL_CALL(AudioSourceComponent_Pause);
		CC_ADD_INTERNAL_CALL(AudioSourceComponent_Stop);
		CC_ADD_INTERNAL_CALL(AudioSourceComponent_SetAudioClip);

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

		CC_ADD_INTERNAL_CALL(TextComponent_SetFont);
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
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTexture);
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
