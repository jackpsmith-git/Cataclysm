#include "ccpch.h"

#include "Panels/CPGamePanel.h"
#include <Cataclysm.h>

#include "Cataclysm/Scene/CCSceneEntityManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "ImGui\CPImGui.h"

namespace Cataclysm
{
	CPGamePanel::CPGamePanel()
	{
	}

	void CPGamePanel::Init()
	{
		Vesuvius::VSFramebufferSpecification spec;
		spec.Attachments = { Vesuvius::VSFramebufferTextureFormat::RGBA8, Vesuvius::VSFramebufferTextureFormat::RED_INTEGER, Vesuvius::VSFramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Vesuvius::VSFramebuffer::Create(spec);
	}

	void CPGamePanel::OnImGuiRender(const CCRef<CCScene>& scene, CCTimestep ts, int state)
	{
		ImVec4 skyboxColor = ImVec4(scene->SkyboxColor.x, scene->SkyboxColor.y, scene->SkyboxColor.z, scene->SkyboxColor.w);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, skyboxColor);
		CPImGui::BeginPanel("Game");
		ImGui::PopStyleColor();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
		{
			auto spec = m_Framebuffer->GetSpecification();
			if (spec.Width != (uint32_t)m_ViewportSize.x || spec.Height != (uint32_t)m_ViewportSize.y)
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_Framebuffer->Bind();
		Vesuvius::VSRenderCommand::SetClearColor(glm::vec4(0, 0, 0, 0));
		Vesuvius::VSRenderCommand::Clear();
		scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		Cataclysm::Entity primaryCameraEntity = CCSceneEntityManager::GetPrimaryCameraEntity(scene.get());
		Cataclysm::IDComponent& idc = Cataclysm::IDComponent();
		if (primaryCameraEntity)
		{
			idc = primaryCameraEntity.GetComponent<IDComponent>();
			if (idc.Enabled)
			{
				scene->UpdateGameRuntime();
			}
			else
			{
				Vesuvius::VSRenderCommand::SetClearColor(glm::vec4(0, 0, 0, 0));
				Vesuvius::VSRenderCommand::Clear();
			}
		}
		else
		{
			Vesuvius::VSRenderCommand::SetClearColor(glm::vec4(0, 0, 0, 0));
			Vesuvius::VSRenderCommand::Clear();

		}

		m_Framebuffer->Unbind();
		CPImGui::Image(m_Framebuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y });

		CPImGui::EndPanel();
	}
}
