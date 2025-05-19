#include "ccpch.h"

#include "GamePanel.h"
#include <Cataclysm.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Cataclysm
{
	GamePanel::GamePanel()
	{
	}

	void GamePanel::Init()
	{
		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);
	}

	void GamePanel::OnImGuiRender(const Ref<Scene>& scene, Timestep ts, int state)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Game");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
		{
			auto spec = m_Framebuffer->GetSpecification();
			if (spec.Width != (uint32_t)m_ViewportSize.x || spec.Height != (uint32_t)m_ViewportSize.y)
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_Framebuffer->Bind();
		Cataclysm::RenderCommand::SetClearColor({ 0, 0, 0, 1 }); // default black
		Cataclysm::RenderCommand::Clear();

		if (state != 0)
		{
			if (scene->GetPrimaryCameraEntity() && scene->GetPrimaryCameraEntity().GetComponent<IDComponent>().Enabled)
			{
				scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				scene->OnRenderRuntime();
			}
			else
			{
				Cataclysm::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
				Cataclysm::RenderCommand::Clear();
			}
		}
		else
		{
			scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			// Render using primary scene camera, even in Edit mode
			Cataclysm::Entity primaryCameraEntity = scene->GetPrimaryCameraEntity();
			if (primaryCameraEntity && primaryCameraEntity.GetComponent<IDComponent>().Enabled)
			{
				auto& cameraComp = primaryCameraEntity.GetComponent<CameraComponent>();
				auto& transform = primaryCameraEntity.GetComponent<TransformComponent>();

				scene->RenderScene(cameraComp.Camera, transform.GetTransform());
			}
			else
			{
				// Optional: clear the screen to black if no camera found
				Cataclysm::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
				Cataclysm::RenderCommand::Clear();
			}
		}
		m_Framebuffer->Unbind();

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(intptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}
}
