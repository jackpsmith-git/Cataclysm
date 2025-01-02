#pragma once

#include "Camera.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "EditorCamera.h"

#include "Cataclysm/Scene/Components.h"
#include "Cataclysm/Renderer/Font.h"

namespace Cataclysm
{
	class Vesuvius
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void StartBatch();
		static void NextBatch();
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);
		
		struct TextParams
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};

		static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID = -1);
		static void DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component, int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	};
}
