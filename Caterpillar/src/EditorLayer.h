#pragma once

#include "Cataclysm.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/BrowserPanel.h"
#include "Panels/FrameDebuggerPanel.h"
#include "Panels/OutputPanel.h"
#include "Panels/GamePanel.h"
#include "Panels/DocumentationPanel.h"
#include "Panels/ProfilerPanel.h"
#include "Panels/WelcomePanel.h"
#include "Panels/SettingsPanel.h"

#include "Cataclysm/Renderer/EditorCamera.h"

namespace Cataclysm
{
	class EditorLayer : public Cataclysm::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Cataclysm::Timestep ts) override;
		virtual void OnImGuiRender(Timestep ts) override;
		void OnEvent(Cataclysm::Event& e) override;
	private:
		bool OnKeyPressed(Cataclysm::KeyPressedEvent& e);
		bool OnMouseButtonPressed(Cataclysm::MouseButtonPressedEvent& e);

		void OnOverlayRender();
		void StatusBar();

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Cataclysm::Ref<Cataclysm::Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();
		void OnScenePause();

		void OnDuplicateEntity();

		void UI_Toolbar();
	private:
		Cataclysm::OrthographicCameraController m_CameraController;

		Cataclysm::Ref<Cataclysm::VertexArray> m_SquareVA;
		Cataclysm::Ref<Cataclysm::Shader> m_FlatColorShader;
		Cataclysm::Ref<Cataclysm::Framebuffer> m_Framebuffer;

		Cataclysm::Ref<Cataclysm::Scene> m_ActiveScene;
		Cataclysm::Ref<Cataclysm::Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
		Cataclysm::Entity m_LogoEntity;
		Cataclysm::Entity m_CameraEntity;
		Cataclysm::Entity m_SecondCamera;

		Cataclysm::Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		Cataclysm::EditorCamera m_EditorCamera;

		Cataclysm::Ref<Cataclysm::Texture2D> m_CheckerboardTexture;
		Cataclysm::Ref<Cataclysm::Texture2D> m_JackPSmithLogoTexture;
		Cataclysm::Ref<Cataclysm::Texture2D> m_CsharpLogo;
		Cataclysm::Ref<Cataclysm::Texture2D> m_VS2022Logo;
		Cataclysm::Ref<Cataclysm::Texture2D> m_LuaLogo;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = 0;

		bool m_ShowPhysicsColliders = false;

		bool m_ShowDocumentationPanel = false;
		bool m_ShowOutputPanel = true;
		bool m_ShowVesuviusPanel = true;
		bool m_ShowFrameDebuggerPanel = true;
		bool m_ShowProfilerPanel = false;
		bool m_ShowWelcomePanel = true;
		bool m_ShowSettingsPanel = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;

		// Panels
		Cataclysm::SceneHierarchyPanel m_SceneHierarchyPanel;
		Scope<BrowserPanel> m_BrowserPanel;
		Scope<FrameDebuggerPanel> m_FrameDebuggerPanel;
		Scope<OutputPanel> m_OutputPanel;
		Scope<GamePanel> m_GamePanel;
		Scope<DocumentationPanel> m_DocumentationPanel;
		Scope<ProfilerPanel> m_ProfilerPanel;
		Scope<WelcomePanel> m_WelcomePanel;
		Scope<SettingsPanel> m_SettingsPanel;

		Cataclysm::Ref<Cataclysm::Texture2D> m_IconPlay, m_IconPause, m_IconStep, m_IconSimulate, m_IconStop, 
			m_IconShowPhysicsColliders, m_IconTranslate, m_IconRotate, m_IconScale, 
			m_IconShowPhysicsCollidersFaded, m_IconTranslateFaded, m_IconRotateFaded, m_IconScaleFaded, m_IconEmpty, m_IconEmptyFaded,
			m_IconClear, m_IconDocumentation, m_IconResetCamera,
			m_IconError, m_IconWarning, m_IconTrace, m_IconInfo,
			m_IconErrorFaded, m_IconWarningFaded, m_IconTraceFaded, m_IconInfoFaded, m_CataclysmLogo;
	};
}
