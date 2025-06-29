#pragma once

#include "VSRenderCommand.h"
#include "VSOrthographicCamera.h"
#include "VSShader.h"

namespace Cataclysm::Vesuvius
{
	class VSRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(VSOrthographicCamera& camera);
		static void EndScene();

		static void Submit(const CCRef<VSShader>& shader, const CCRef<VSVertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static VSRendererAPI::VSAPI GetAPI() { return VSRendererAPI::GetAPI(); }

	private:
		struct VSSceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static VSSceneData* m_SceneData;
	};
}
