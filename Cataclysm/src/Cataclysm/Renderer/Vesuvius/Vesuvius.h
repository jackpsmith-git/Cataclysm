#pragma once

// renderer
#include "Cataclysm/Renderer/Camera.h"
#include "Cataclysm/Renderer/OrthographicCamera.h"
#include "Cataclysm/Renderer/Texture.h"
#include "Cataclysm/Renderer/EditorCamera.h"
#include "Cataclysm/Renderer/Font.h"
#include "Cataclysm/Renderer/Vesuvius/VesuviusStats.h"
#include "Cataclysm/Renderer/TextParams.h"

// scene
#include "Cataclysm/ECS/Components.h"

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

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D> texture, float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);
	
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID = -1);
		static void DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component, int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		static void ResetStats();
		static VesuviusStats GetStats();
	};
}
