#pragma once

#include "Cataclysm.h"
#include "Panels/CPSceneHierarchyPanel.h"
#include "Panels/CPBrowserPanel.h"
#include "Panels/CPFrameDebuggerPanel.h"
#include "Panels/CPOutputPanel.h"
#include "Panels/CPGamePanel.h"
#include "Panels/CPProfilerPanel.h"
#include "Panels/CPWelcomePanel.h"
#include "Panels/CPSettingsPanel.h"
#include "Panels/CPVesuviusPanel.h"
#include "Panels/CPTestPanel.h"

#include "Cataclysm/Vesuvius/VSEditorCamera.h"

namespace Cataclysm
{
	class CPEditorLayer : public Cataclysm::CCLayer
	{
	public:
		CPEditorLayer();
		virtual ~CPEditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Cataclysm::CCTimestep ts) override;
		virtual void OnImGuiRender(CCTimestep ts) override;
		void OnEvent(Cataclysm::CCEvent& e) override;
	private:
		bool OnKeyPressed(Cataclysm::CCKeyPressedEvent& e);
		bool OnMouseButtonPressed(Cataclysm::CCMouseButtonPressedEvent& e);

		void ResizeScene();

		void OnOverlayRender();
		void RenderPanels(CCTimestep ts);
		void RenderScene();

		void MenuBar();
		void StatusBar();
		void UI_Toolbar();
		void RenderGizmos();

		void ResetLayout();

		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();
		void CloseProject();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();
		void OnScenePause();

		void OnDuplicateEntity();

		std::string OpenFolderDialog(HWND owner = nullptr);
		void CreateProject(std::string directory, std::string projectName);
	private:
		bool m_ProjectOpen = false;
		bool m_ReloadLayout = false;
		Vesuvius::VSOrthographicCameraController m_CameraController;

		Cataclysm::CCRef<Vesuvius::VSVertexArray> m_SquareVA;
		Cataclysm::CCRef<Vesuvius::VSShader> m_FlatColorShader;
		Cataclysm::CCRef<Vesuvius::VSFramebuffer> m_Framebuffer;

		Cataclysm::CCRef<Cataclysm::CCScene> m_ActiveScene;
		Cataclysm::CCRef<Cataclysm::CCScene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;

		Cataclysm::Entity m_LogoEntity;
		Cataclysm::Entity m_CameraEntity;
		Cataclysm::Entity m_SecondCamera;
		Cataclysm::Entity m_HoveredEntity;

		Vesuvius::VSEditorCamera m_EditorCamera;
		bool m_PrimaryCamera = true;

		Cataclysm::CCRef<Vesuvius::VSTexture2D> m_CsharpLogo;
		Cataclysm::CCRef<Vesuvius::VSTexture2D> m_VS2022Logo;
		Cataclysm::CCRef<Vesuvius::VSTexture2D> m_LuaLogo;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = 0;

		bool m_ShowPhysicsColliders = false;
		bool m_ShowOutputPanel = true;
		bool m_ShowVesuviusPanel = false;
		bool m_ShowFrameDebuggerPanel = false;
		bool m_ShowProfilerPanel = false;
		bool m_ShowWelcomePanel = true;
		bool m_ShowSettingsPanel = false;

		enum class CPSceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};

		CPSceneState m_SceneState = CPSceneState::Edit;

		Cataclysm::CPSceneHierarchyPanel m_SceneHierarchyPanel;
		CCScope<CPBrowserPanel> m_BrowserPanel;
		CCScope<CPFrameDebuggerPanel> m_FrameDebuggerPanel;
		CCScope<CPOutputPanel> m_OutputPanel;
		CCScope<CPGamePanel> m_GamePanel;
		CCScope<CPProfilerPanel> m_ProfilerPanel;
		CCScope<CPWelcomePanel> m_WelcomePanel;
		CCScope<CPSettingsPanel> m_SettingsPanel;
		CCScope<CPVesuviusPanel> m_VesuviusPanel;
		CCScope<CPTestPanel> m_TestPanel;
	};
}
