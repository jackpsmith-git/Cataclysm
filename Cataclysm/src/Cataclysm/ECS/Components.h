#pragma once

#include "Cataclysm/Scene/SceneCamera.h"
#include "Cataclysm/Core/UUID.h"
#include "Cataclysm/Renderer/Texture.h"
#include "Cataclysm/Renderer/Font.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cataclysm
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		bool Enabled = true;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GlobalTransform = 1.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		void Reset()
		{
			Translation = { 0.0f, 0.0f, 0.0f };
			Rotation = { 0.0f, 0.0f, 0.0f };
			Scale = { 1.0f, 1.0f, 1.0f };
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		void Reset()
		{
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Texture = NULL;
			TilingFactor = 1.0f;
		}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;

		void Reset()
		{
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Thickness = 1.0f;
			Fade = 0.005f;
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void Reset()
		{
			Primary = true;
			FixedAspectRatio = false;

			Camera.SetProjectionType(Cataclysm::SceneCamera::ProjectionType::Orthographic);

			Camera.SetOrthographicSize(10.0f);
			Camera.SetOrthographicNearClip(-1.0f);
			Camera.SetOrthographicFarClip(1.0f);

			Camera.SetPerspectiveVerticalFOV(45.0f);
			Camera.SetPerspectiveNearClip(0.01f);
			Camera.SetPerspectiveFarClip(1000.0f);
		}
	};

	struct MonoScriptComponent
	{
		std::string ClassName;

		MonoScriptComponent() = default;
		MonoScriptComponent(const MonoScriptComponent&) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct Rigidbody2DComponent
	{
	public:
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;
		float GravityScale = 1.0f;
		float Mass = 1.0f;

		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

		void Reset()
		{
			Type = BodyType::Static;
			FixedRotation = false;
			GravityScale = 1.0f;
			Mass = 1.0f;
		}
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.5f;
		float RestitutionThreshold = 0.5f;

		bool IsTrigger = false;

		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

		void Reset()
		{
			Offset = { 0.0f, 0.0f };
			Size = { 0.5f, 0.5f };
			Density = 1.0f;
			Friction = 0.5f;
			Restitution = 0.5f;
			RestitutionThreshold = 0.5f;
			IsTrigger = false;
		}
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.5f;
		float RestitutionThreshold = 0.5f;

		bool IsTrigger = false;

		void* RuntimeFixture = nullptr;
		
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

		void Reset()
		{
			Offset = { 0.0f, 0.0f };
			Radius = 0.5f;
			Density = 1.0f;
			Friction = 0.5f;
			Restitution = 0.5f;
			RestitutionThreshold = 0.5f;
			IsTrigger = false;
		}
	};

	struct TextComponent
	{
		std::string TextString;
		Ref<Font> FontAsset = Font::GetDefault();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
		std::string FontPath = "assets/fonts/opensans/OpenSans-Regular.ttf";

		TextComponent() = default;
		TextComponent(const TextComponent&) = default;

		void Reset()
		{
			TextString.clear();
			FontAsset = Font::GetDefault();
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Kerning = 0.0f;
			LineSpacing = 0.0f;
			FontPath = "assets/fonts/opensans/OpenSans-Regular.ttf";
		}
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent,
		CircleRendererComponent, CameraComponent, MonoScriptComponent,
		NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent,
		CircleCollider2DComponent, TextComponent>; 
}
