#include "ccpch.h"
#include "Cataclysm/Vesuvius/VSRenderer.h"

#include "Cataclysm/Vesuvius/VSRenderer2D.h"

#include "Platform/OpenGL/VSOpenGLShader.h"

namespace Cataclysm::Vesuvius
{
	VSRenderer::VSSceneData* VSRenderer::m_SceneData = new VSRenderer::VSSceneData;

	void VSRenderer::Init()
	{
		VSRenderCommand::Init();
		VSRenderer2D::Init();
	}

	void VSRenderer::Shutdown()
	{
		VSRenderer2D::Shutdown();
	}

	void VSRenderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		VSRenderCommand::SetViewport(0, 0, width, height);
	}

	void VSRenderer::BeginScene(VSOrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void VSRenderer::EndScene()
	{
	}

	void VSRenderer::Submit(const CCRef<VSShader>& shader, const CCRef<VSVertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		VSRenderCommand::DrawIndexed(vertexArray);
	}
}
