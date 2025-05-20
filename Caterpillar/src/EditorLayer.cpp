#include "ccpch.h"
#include "EditorLayer.h"

// imgui
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// cataclysm
#include "Cataclysm/Scene/SceneSerializer.h"
#include "Cataclysm/Scripting/ScriptEngine.h"
#include "Cataclysm/Audio/AudioEngine.h"
#include "Cataclysm/Math/Math.h"
#include "Cataclysm/Utils/PlatformUtils.h"
#include "Cataclysm/Renderer/Font.h"

// imguizmo
#include "ImGuizmo.h"

#include <cstdlib>


namespace Cataclysm
{
	static Ref<Font> s_Font;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		s_Font = Font::GetDefault();
	}

	void EditorLayer::OnAttach()
	{
		CC_PROFILE_FUNCTION();

		AudioEngine::Init();

		// m_CheckerboardTexture = Cataclysm::Texture2D::Create("assets/textures/Checkerboard.png");
		// m_JackPSmithLogoTexture = Cataclysm::Texture2D::Create("assets/textures/JackPSmithLogo.png");
		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconPause = Texture2D::Create("Resources/Icons/PauseButton.png");
		m_IconStep = Texture2D::Create("Resources/Icons/StepButton.png");
		m_IconSimulate = Texture2D::Create("Resources/Icons/SimulateButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");
		m_IconShowPhysicsColliders = Texture2D::Create("Resources/Icons/ShowPhysicsColliders.png");
		m_IconTranslate = Texture2D::Create("Resources/Icons/TranslateIcon.png");
		m_IconRotate = Texture2D::Create("Resources/Icons/RotateIcon.png");
		m_IconScale = Texture2D::Create("Resources/Icons/ScaleIcon.png");
		m_IconShowPhysicsCollidersFaded = Texture2D::Create("Resources/Icons/ShowPhysicsCollidersFaded.png");
		m_IconTranslateFaded = Texture2D::Create("Resources/Icons/TranslateIconFaded.png");
		m_IconRotateFaded = Texture2D::Create("Resources/Icons/RotateIconFaded.png");
		m_IconScaleFaded = Texture2D::Create("Resources/Icons/ScaleIconFaded.png");
		m_IconEmpty = Texture2D::Create("Resources/Icons/EmptyIcon.png");
		m_IconEmptyFaded = Texture2D::Create("Resources/Icons/EmptyIconFaded.png");
		m_IconClear = Texture2D::Create("Resources/Icons/ClearIcon.png");
		m_IconError = Texture2D::Create("Resources/Icons/ErrorIcon.png");
		m_IconErrorFaded = Texture2D::Create("Resources/Icons/ErrorIconFaded.png");
		m_IconWarning = Texture2D::Create("Resources/Icons/WarningIcon.png");
		m_IconWarningFaded = Texture2D::Create("Resources/Icons/WarningIconFaded.png");
		m_IconTrace = Texture2D::Create("Resources/Icons/TraceIcon.png");
		m_IconTraceFaded = Texture2D::Create("Resources/Icons/TraceIconFaded.png");
		m_IconInfo = Texture2D::Create("Resources/Icons/InfoIcon.png");
		m_IconInfoFaded = Texture2D::Create("Resources/Icons/InfoIconFaded.png");
		m_CataclysmLogo = Texture2D::Create("Resources/Icons/CataclysmLogo.png");
		m_IconDocumentation = Texture2D::Create("Resources/Icons/DocumentationIcon.png");
		m_IconResetCamera = Texture2D::Create("Resources/Icons/ResetCameraIcon.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, Cataclysm::FramebufferTextureFormat::RED_INTEGER, Cataclysm::FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Cataclysm::Framebuffer::Create(fbSpec);

		m_EditorScene = Cataclysm::CreateRef<Cataclysm::Scene>();
		m_ActiveScene = m_EditorScene;

		m_GamePanel = CreateScope<GamePanel>();
		m_GamePanel->Init();

		auto commandLineArgs = Cataclysm::Application::Get().GetSpecification().CommandLineArgs;

		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			if (!OpenProject())
				Application::Get().Close();
		}

		m_ProfilerPanel = CreateScope<ProfilerPanel>();
		m_ProfilerPanel->Init();

		m_EditorCamera = Cataclysm::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Cataclysm::Vesuvius::SetLineWidth(4.0f);
	}

	void EditorLayer::OnDetach()
	{
		CC_PROFILE_FUNCTION();

		AudioEngine::Shutdown();
	}

	void EditorLayer::OnUpdate(Cataclysm::Timestep ts)
	{
		CC_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		// Resize
		if (Cataclysm::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		// Render
		Cataclysm::Vesuvius::ResetStats();
		m_Framebuffer->Bind();
		Cataclysm::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Cataclysm::RenderCommand::Clear();

		// Clear entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Update Scene
		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				if (m_ViewportFocused)
					m_CameraController.OnUpdate(ts);

				m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Simulate:
			{
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Cataclysm::Entity() : Cataclysm::Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Cataclysm::Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;
			Cataclysm::Vesuvius::BeginScene(camera.GetComponent<Cataclysm::CameraComponent>().Camera, camera.GetComponent<Cataclysm::TransformComponent>().GetTransform());
		}
		else
		{
			Cataclysm::Vesuvius::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					glm::mat4 transform = tc.GlobalTransform;

					glm::vec3 translation, rotation, scale;
					Cataclysm::Math::DecomposeTransform(transform, translation, rotation, scale);

					float cosAngle = cos(rotation.z);
					float sinAngle = sin(rotation.z);

					// Rotate offset vector in 2D plane (X,Y)
					glm::vec2 rotatedOffset = {
						bc2d.Offset.x * cosAngle - bc2d.Offset.y * sinAngle,
						bc2d.Offset.x * sinAngle + bc2d.Offset.y * cosAngle
					};

					glm::vec3 colliderTranslation = translation + glm::vec3(rotatedOffset, 0.0f);
					glm::vec3 colliderScale = scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), colliderTranslation)
						* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1))
						* glm::scale(glm::mat4(1.0f), colliderScale);

					Vesuvius::DrawRect(colliderTransform, glm::vec4(0, 1, 0, 1));
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::mat4 transform = tc.GlobalTransform;

					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					float cosAngle = cos(rotation.z);
					float sinAngle = sin(rotation.z);

					// Rotate offset vector in 2D plane (X,Y)
					glm::vec2 rotatedOffset = {
						cc2d.Offset.x * cosAngle - cc2d.Offset.y * sinAngle,
						cc2d.Offset.x * sinAngle + cc2d.Offset.y * cosAngle
					};

					glm::vec3 colliderTranslation = translation + glm::vec3(rotatedOffset, 0.0f);					float maxScale = glm::max(scale.x, scale.y);
					glm::vec3 colliderScale = glm::vec3(cc2d.Radius * 2.0f * maxScale, cc2d.Radius * 2.0f * maxScale, 1.0f);

					glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), colliderTranslation)
						* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1))
						* glm::scale(glm::mat4(1.0f), colliderScale);

					Vesuvius::DrawCircleDebug(colliderTransform, glm::vec4(0, 1, 0, 1), 0.05f);
				}
			}
		}

		if (Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			const Cataclysm::TransformComponent& transform = selectedEntity.GetComponent<Cataclysm::TransformComponent>();
			Cataclysm::Vesuvius::DrawRect(transform.GlobalTransform, glm::vec4(1.0f, 0.65f, 0.0f, 1.0f));
		}

		Cataclysm::Vesuvius::EndScene();
	}

	void EditorLayer::OnImGuiRender(Timestep ts)
	{
		CC_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistent = true;
		bool opt_fullscreen = opt_fullscreen_persistent;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoWindowMenuButton;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dockspace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 200.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) 
					Cataclysm::Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::BeginMenu("Panels"))
				{
					ImGui::MenuItem("Documentation", nullptr, &m_ShowDocumentationPanel);
					ImGui::MenuItem("Frame", nullptr, &m_ShowFrameDebuggerPanel);
					ImGui::MenuItem("Output", nullptr, &m_ShowOutputPanel);
					ImGui::MenuItem("Profiler", nullptr, &m_ShowProfilerPanel);
					ImGui::MenuItem("Settings", nullptr, &m_ShowSettingsPanel);
					ImGui::MenuItem("Vesuvius", nullptr, &m_ShowVesuviusPanel);

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Reset Editor Camera"))
					m_EditorCamera.Reset();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Online Documentation"))
				{
					const std::string url = "https://jackpsmith.com/cataclysm/docs/";
					std::string command = "start " + url;
					system(command.c_str());
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_BrowserPanel->OnImGuiRender();

		if (m_ShowFrameDebuggerPanel)
			m_FrameDebuggerPanel->OnImGuiRender(ts);
		
		if (m_ShowOutputPanel)
			m_OutputPanel->OnImGuiRender(m_IconClear, m_IconError, m_IconWarning, m_IconTrace, m_IconInfo, m_IconErrorFaded, m_IconWarningFaded, m_IconTraceFaded, m_IconInfoFaded);

		if (m_ShowDocumentationPanel)
			m_DocumentationPanel->OnImGuiRender();

		if (m_ShowProfilerPanel)
			m_ProfilerPanel->OnImGuiRender();

		if (m_ShowSettingsPanel)
			m_SettingsPanel->OnImGuiRender();

		// if (m_ShowWelcomePanel)
		//	m_WelcomePanel->OnImGuiRender(m_ShowWelcomePanel);

		if (m_ShowVesuviusPanel)
		{
			ImGui::Begin("Vesuvius");

			auto stats = Cataclysm::Vesuvius::GetStats();
			ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
			ImGui::Text("Quads:	   %d", stats.QuadCount);
			ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());

			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Scene");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Cataclysm::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			
			// Runtime camera from entity
			//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			if (m_SceneState == SceneState::Edit)
			{
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<Cataclysm::TransformComponent>();

				UUID entityID = selectedEntity.GetUUID();

				// Step 1: Get parent's world transform
				UUID parentID = m_ActiveScene->GetParent(entityID);
				glm::mat4 parentTransform = glm::mat4(1.0f);
				if (parentID)
					parentTransform = m_ActiveScene->GetWorldTransform(parentID);

				// Step 2: Get entity's current world transform
				glm::mat4 worldTransform = m_ActiveScene->GetWorldTransform(entityID);

				// Step 3: Pass world transform to ImGuizmo
			
				// Snapping
				bool snap = Cataclysm::Input::IsKeyDown(Cataclysm::Key::LeftControl) || Cataclysm::Input::IsKeyDown(Cataclysm::Key::RightControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };
				ImGuizmo::Manipulate(
					glm::value_ptr(cameraView),
					glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::WORLD, // WORLD because we’re using global matrix
					glm::value_ptr(worldTransform),
					nullptr,
					snap ? snapValues : nullptr
				);

				// Step 4: Convert manipulated world transform back into local
				if (ImGuizmo::IsUsing())
				{
					m_ActiveScene->SetWorldTransform(selectedEntity.GetUUID(), worldTransform);
				}
			}
			else
			{
				Entity cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();

				auto& cameraComponent = cameraEntity.GetComponent<CameraComponent>();
				auto& transformComponent = cameraEntity.GetComponent<TransformComponent>();

				glm::mat4 cameraProjection = cameraComponent.Camera.GetProjection();
				glm::mat4 cameraTransform = transformComponent.GetTransform();
				glm::mat4 cameraView = glm::inverse(cameraTransform);

				// Entity transform
				auto& tc = selectedEntity.GetComponent<Cataclysm::TransformComponent>();

				UUID entityID = selectedEntity.GetUUID();

				// Step 1: Get parent's world transform
				UUID parentID = m_ActiveScene->GetParent(entityID);
				glm::mat4 parentTransform = glm::mat4(1.0f);
				if (parentID)
					parentTransform = m_ActiveScene->GetWorldTransform(parentID);

				// Step 2: Get entity's current world transform
				glm::mat4 worldTransform = m_ActiveScene->GetWorldTransform(entityID);

				// Step 3: Pass world transform to ImGuizmo

				// Snapping
				bool snap = Cataclysm::Input::IsKeyDown(Cataclysm::Key::LeftControl) || Cataclysm::Input::IsKeyDown(Cataclysm::Key::RightControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };
				ImGuizmo::Manipulate(
					glm::value_ptr(cameraView),
					glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::WORLD, // WORLD because we’re using global matrix
					glm::value_ptr(worldTransform),
					nullptr,
					snap ? snapValues : nullptr
				);

				// Step 4: Convert manipulated world transform back into local
				if (ImGuizmo::IsUsing())
				{
					m_ActiveScene->SetWorldTransform(selectedEntity.GetUUID(), worldTransform);
				}

			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		m_GamePanel->OnImGuiRender(m_ActiveScene, ts, static_cast<int>(m_SceneState));

		UI_Toolbar();
		StatusBar();

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
		
		ImGui::GetStyle().WindowMinSize.y = 25.0f;
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;
		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		if (hasPlayButton)
		{
			Cataclysm::Ref<Cataclysm::Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		//if (hasSimulateButton)
		//{
		//	if (hasPlayButton)
		//		ImGui::SameLine();

		//	Cataclysm::Ref<Cataclysm::Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
		//	//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		//	if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		//	{
		//		if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
		//			OnSceneSimulate();
		//		else if (m_SceneState == SceneState::Simulate)
		//			OnSceneStop();
		//	}
		//}
		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					bool isPaused = m_ActiveScene->IsPaused();
					if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step();
					}
				}
			}
		}

		ImGui::PopStyleVar();

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ((size * 8) + 10));

		if (ImGui::ImageButton((ImTextureID)(uint64_t)m_IconResetCamera->GetRendererID(), ImVec2(size * 2.315, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		{
			m_EditorCamera.Reset();
		}
		ImGui::SameLine();

		ImGui::Spacing();
		ImGui::SameLine();

		if (m_ShowPhysicsColliders)
		{
			Ref<Texture2D> physicsIcon = m_IconShowPhysicsColliders;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)physicsIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_ShowPhysicsColliders = !(m_ShowPhysicsColliders);
			}
		}
		else
		{
			Ref<Texture2D> physicsIcon = m_IconShowPhysicsCollidersFaded;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)physicsIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_ShowPhysicsColliders = !(m_ShowPhysicsColliders);
			}
		}

		ImGui::SameLine();
		ImGui::Spacing();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		ImGui::SameLine();
		if (m_GizmoType == -1)
		{
			Ref<Texture2D> emptyIcon = m_IconEmpty;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)emptyIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = -1;
			}
		}
		else
		{
			Ref<Texture2D> emptyIcon = m_IconEmptyFaded;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)emptyIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = -1;
			}
		}
		
		ImGui::SameLine();
		if (m_GizmoType == 0)
		{
			Ref<Texture2D> translateIcon = m_IconTranslate;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)translateIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = 0;
			}
		}
		else
		{
			Ref<Texture2D> translateIcon = m_IconTranslateFaded;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)translateIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = 0;
			}
		}

		ImGui::SameLine();
		if (m_GizmoType == 1)
		{
			Ref<Texture2D> rotateIcon = m_IconRotate;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)rotateIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = 1;
			}
		}
		else
		{
			Ref<Texture2D> rotateIcon = m_IconRotateFaded;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)rotateIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = 1;
			}
		}

		ImGui::SameLine();
		if (m_GizmoType == 2)
		{
			Ref<Texture2D> scaleIcon = m_IconScale;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)scaleIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = 2;
			}
		}
		else
		{
			Ref<Texture2D> scaleIcon = m_IconScaleFaded;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)scaleIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_GizmoType = 2;
			}
		}


		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::StatusBar()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		float barHeight = 24.0f;

		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - barHeight));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, barHeight));
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 255));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 100, 100, 255));

		ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionMax().x, 0), ImGuiCond_Always);
		ImGui::Begin("##statusbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		
		ImGui::Spacing();
		ImGui::SameLine();

		ImGui::Image((ImTextureID)(uint64_t)m_CataclysmLogo->GetRendererID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SameLine();
		ImGui::Spacing();

		ImGui::SameLine();
		ImGui::Text("Cataclysm v1.0.1 - Copyright 2025 Jack P Smith");
		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 24);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((ImTextureID)(uint64_t)m_IconDocumentation->GetRendererID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0)))
		{
			const std::string url = "https://jackpsmith.com/cataclysm/docs/";
			std::string command = "start " + url;
			system(command.c_str());
		}
		ImGui::PopStyleColor();
		ImGui::End();
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(4);
	}

	void EditorLayer::OnScenePlay()
	{
		ImGui::SetWindowFocus("Game");
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;
		m_ActiveScene = Cataclysm::Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();
		m_SceneState = SceneState::Simulate;
		m_ActiveScene = Cataclysm::Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		CC_CORE_ASSERT("Unknown scene state!", m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);
		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = m_EditorScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}

	void EditorLayer::OnEvent(Cataclysm::Event& e)
	{
		m_CameraController.OnEvent(e);

		if (m_SceneState == SceneState::Edit)
			m_EditorCamera.OnEvent(e);

		Cataclysm::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Cataclysm::KeyPressedEvent>(CC_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<Cataclysm::MouseButtonPressedEvent>(CC_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(Cataclysm::KeyPressedEvent& e)
	{
		//Shortcuts
		if (e.IsRepeat())
			return false;

		bool control = Cataclysm::Input::IsKeyDown(Cataclysm::Key::LeftControl) || Cataclysm::Input::IsKeyDown(Cataclysm::Key::RightControl);
		bool shift = Cataclysm::Input::IsKeyDown(Cataclysm::Key::LeftShift) || Cataclysm::Input::IsKeyDown(Cataclysm::Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Cataclysm::Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Cataclysm::Key::O:
			{
				if (control)
					OpenProject();

				break;
			}
			case Cataclysm::Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}

			// Scene Commands
			case Cataclysm::Key::D:
			{
				if (control)
					OnDuplicateEntity();

				break;
			}

			// Gizmos
			case Cataclysm::Key::Q:
			{
				m_GizmoType = -1;
				break;
			}
			case Cataclysm::Key::W:
			{
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Cataclysm::Key::E:
			{
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Cataclysm::Key::R:
			{
				if (control)
				{
					ScriptEngine::ReloadAssembly();
				}
				else
				{
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
			case Key::Delete:
			{
				if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
				{
					Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntity)
					{
						m_SceneHierarchyPanel.SetSelectedEntity({});
						m_ActiveScene->DestroyEntity(selectedEntity);
					}
				}
				break;
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(Cataclysm::MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Cataclysm::Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && m_ViewportFocused && !ImGuizmo::IsOver())
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Cataclysm Project (*.ccproj)\0*.ccproj\0", ".ccproj");

		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			m_BrowserPanel = CreateScope<BrowserPanel>();
			m_FrameDebuggerPanel = CreateScope<FrameDebuggerPanel>();
			m_OutputPanel = CreateScope<OutputPanel>();
		}
	}

	void EditorLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = Cataclysm::CreateRef<Cataclysm::Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = Cataclysm::FileDialogs::OpenFile("Cataclysm Scene (*.ccscene)\0*.ccscene\0", ".ccscene");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".ccscene")
		{
			CC_WARN("[EditorLayer::OpenScene] Could not load '{0}' - not a scene file", path.filename().string());
			return;
		}

		Cataclysm::Ref<Cataclysm::Scene> newScene = Cataclysm::CreateRef<Cataclysm::Scene>();
		Cataclysm::SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_SceneHierarchyPanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_SceneState == SceneState::Edit)
		{
			if (!m_EditorScenePath.empty())
				SerializeScene(m_ActiveScene, m_EditorScenePath);
			else
				SaveSceneAs();
		}
		else if (m_SceneState == SceneState::Play)
		{
			CC_CORE_WARN("Cannot save scene while in Play Mode!");
		}
		else if (m_SceneState == SceneState::Simulate)
		{
			CC_CORE_WARN("Cannot save scene while in Simulate Mode!");
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = Cataclysm::FileDialogs::SaveFile("Cataclysm Scene (*.ccscene)\0*.ccscene\0", ".ccscene");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Cataclysm::Ref<Cataclysm::Scene> scene, const std::filesystem::path& path)
	{
		Cataclysm::SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}
}
