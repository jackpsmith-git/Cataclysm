#include "ccpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Cataclysm/Core/UUID.h"
#include "Cataclysm/Input/KeyCodes.h"
#include "Cataclysm/Input/Input.h"

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


	///////////////////////////////////////////
	/////////////// RIGIDBODY2D ///////////////
	///////////////////////////////////////////

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
		CC_ADD_INTERNAL_CALL(Entity_HasComponent);
		CC_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		CC_ADD_INTERNAL_CALL(Entity_GetEnabled);
		CC_ADD_INTERNAL_CALL(Entity_SetEnabled);
		CC_ADD_INTERNAL_CALL(Entity_AddBoxCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCameraComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCircleCollider2DComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddCircleRendererComponent);		
		CC_ADD_INTERNAL_CALL(Entity_AddRigidbody2DComponent);		
		CC_ADD_INTERNAL_CALL(Entity_AddSpriteRendererComponent);
		CC_ADD_INTERNAL_CALL(Entity_AddTextComponent);

		CC_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		CC_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		CC_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		CC_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		CC_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetLinearVelocity);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
		CC_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);

		CC_ADD_INTERNAL_CALL(TextComponent_GetText);
		CC_ADD_INTERNAL_CALL(TextComponent_SetText);
		CC_ADD_INTERNAL_CALL(TextComponent_GetColor);
		CC_ADD_INTERNAL_CALL(TextComponent_SetColor);
		CC_ADD_INTERNAL_CALL(TextComponent_GetKerning);
		CC_ADD_INTERNAL_CALL(TextComponent_SetKerning);
		CC_ADD_INTERNAL_CALL(TextComponent_GetLineSpacing);
		CC_ADD_INTERNAL_CALL(TextComponent_SetLineSpacing);

		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);		
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		CC_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);

		CC_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		CC_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);

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
		
		CC_ADD_INTERNAL_CALL(Input_IsKeyDown);
		CC_ADD_INTERNAL_CALL(Input_IsMouseButtonDown);
		CC_ADD_INTERNAL_CALL(Input_GetMousePosition);
	}
}
