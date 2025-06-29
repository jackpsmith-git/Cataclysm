#include "ccpch.h"
#include "../include/GameLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/include/GLFW/glfw3.h>

#include "Cataclysm/Scene/CCSceneSerializer.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/Audio/CCAudioEngine.h"
#include "Cataclysm/Math/CCMath.h"
#include "Cataclysm/Utils/CCPlatformUtils.h"
#include "Cataclysm/Vesuvius/VSFont.h"
#include "Cataclysm/Asset/CCAssetSystem.h"

#include <cstdlib>

namespace Cataclysm
{
	GameLayer::GameLayer() : CCLayer("GameLayer") {}

	void GameLayer::OnAttach()
	{
		CCScriptEngine::InitGame();
		CCAudioEngine::Init();

		Vesuvius::VSFramebufferSpecification fbSpec;
		fbSpec.Attachments = { Vesuvius::VSFramebufferTextureFormat::RGBA8, Vesuvius::VSFramebufferTextureFormat::RED_INTEGER, Vesuvius::VSFramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Vesuvius::VSFramebuffer::Create(fbSpec);

		m_Scene = Cataclysm::CreateRef<Cataclysm::CCScene>();

		auto commandLineArgs = Cataclysm::CCApplication::Get().GetSpecification().CommandLineArgs;

		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenGame(projectFilePath);
		}
		else
		{
			if (!OpenGame())
				CCApplication::Get().Close();
		}

		Vesuvius::VSRenderer2D::SetLineWidth(4.0f);
		m_Scene->OnRuntimeStart();
	}

	bool GameLayer::OpenGame()
	{
		std::string filepath = CCFileDialogs::OpenFile("Cataclysm Project (*.ccproj)\0*.ccproj\0", ".ccproj");

		if (filepath.empty())
			return false;

		OpenGame(filepath);
		return true;
	}

	void GameLayer::OpenGame(const std::filesystem::path& path)
	{
		if (CCProject::Load(path))
		{
			CCScriptEngine::LoadProjectScripts();
			CCAssetSystem::ImportAssets(CCProject::GetAssetDirectory());

			auto startScenePath = CCProject::GetAssetFileSystemPath(CCProject::GetActive()->GetConfig().StartScene);

			Cataclysm::CCRef<Cataclysm::CCScene> newScene = Cataclysm::CreateRef<Cataclysm::CCScene>();
			Cataclysm::CCSceneSerializer serializer(newScene);
			if (serializer.Deserialize(startScenePath.string()))
			{
				m_Scene = newScene;
			}
		}
	}

	void GameLayer::OnDetach()
	{
		m_Scene->OnRuntimeStop();
		CCAudioEngine::Shutdown();
		CCScriptEngine::Shutdown();
	}

	void GameLayer::OnUpdate(Cataclysm::CCTimestep ts)
	{
		if (m_ViewportSize.x <= 0.0f || m_ViewportSize.y <= 0.0f)
			return;

		m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		Vesuvius::VSRenderer2D::ResetStats();
		m_Framebuffer->Bind();
		Vesuvius::VSRenderCommand::SetClearColor({ 0, 0, 0, 0 });
		Vesuvius::VSRenderCommand::Clear();
		m_Framebuffer->ClearAttachment(1, -1);
		m_Scene->UpdateGameRuntime();
		m_Framebuffer->Unbind();
	}

	void GameLayer::OnImGuiRender(Cataclysm::CCTimestep ts)
	{	
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;
		
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 200.0f;
		
		style.WindowMinSize.x = minWinSizeX;
		
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar(3);
	}
}
