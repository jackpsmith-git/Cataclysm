#pragma once

// renderer
#include "Cataclysm/Vesuvius/VSCamera.h"
#include "Cataclysm/Vesuvius/VSOrthographicCamera.h"
#include "Cataclysm/Vesuvius/VSTexture.h"
#include "Cataclysm/Vesuvius/VSEditorCamera.h"
#include "Cataclysm/Vesuvius/VSFont.h"
#include "Cataclysm/Vesuvius/VSRenderer2DStats.h"
#include "Cataclysm/Vesuvius/VSTextParams.h"

// scene
#include "Cataclysm/ECS/CCComponents.h"

namespace Cataclysm::Vesuvius
{
	class VSRenderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const VSCamera& camera, const glm::mat4& transform);
		static void BeginScene(const VSEditorCamera& camera);
		static void BeginScene(const VSOrthographicCamera& camera);

		static void StartBatch();
		static void NextBatch();
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const CCRef<VSTexture2D> texture, float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawSprite(const glm::mat4& transform, SpriteRenderer& src, int entityID);
	
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		static void DrawCircleDebug(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void DrawString(const std::string& string, CCRef<VSFont> font, const glm::mat4& transform, const VSTextParams& textParams, int entityID = -1);
		static void DrawString(const std::string& string, const glm::mat4& transform, const Text& component, int entityID = -1);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		static void ResetStats();
		static VSRenderer2DStats GetStats();
	};
}
