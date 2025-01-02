#include "ccpch.h"
#include "EditorLayer.h"

// imgui
#include <imgui/imgui.h>

// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// cataclysm
#include "Cataclysm/Scene/SceneSerializer.h"
#include "Cataclysm/Scripting/ScriptEngine.h"
#include "Cataclysm/Math/Math.h"
#include "Cataclysm/Utils/PlatformUtils.h"
#include "Cataclysm/Renderer/Font.h"

// imguizmo
#include "ImGuizmo.h"


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

		m_CheckerboardTexture = Cataclysm::Texture2D::Create("assets/textures/Checkerboard.png");
		m_JackPSmithLogoTexture = Cataclysm::Texture2D::Create("assets/textures/JackPSmithLogo.png");
		m_IconPlay = Cataclysm::Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconPause = Cataclysm::Texture2D::Create("Resources/Icons/PauseButton.png");
		m_IconStep = Cataclysm::Texture2D::Create("Resources/Icons/StepButton.png");
		m_IconSimulate = Cataclysm::Texture2D::Create("Resources/Icons/SimulateButton.png");
		m_IconStop = Cataclysm::Texture2D::Create("Resources/Icons/StopButton.png");

		Cataclysm::FramebufferSpecification fbSpec;
		fbSpec.Attachments = { Cataclysm::FramebufferTextureFormat::RGBA8, Cataclysm::FramebufferTextureFormat::RED_INTEGER, Cataclysm::FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Cataclysm::Framebuffer::Create(fbSpec);

		m_EditorScene = Cataclysm::CreateRef<Cataclysm::Scene>();
		m_ActiveScene = m_EditorScene;

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

		m_EditorCamera = Cataclysm::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Cataclysm::Vesuvius::SetLineWidth(4.0f);
	}

	void EditorLayer::OnDetach()
	{
		CC_PROFILE_FUNCTION();

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
				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
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
				auto view = m_ActiveScene->GetAllEntitiesWith<Cataclysm::TransformComponent, Cataclysm::BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<Cataclysm::TransformComponent, Cataclysm::BoxCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);
					Cataclysm::Vesuvius::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}
			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<Cataclysm::TransformComponent, Cataclysm::CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<Cataclysm::TransformComponent, Cataclysm::CircleCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);
					Cataclysm::Vesuvius::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}

		if (Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			const Cataclysm::TransformComponent& transform = selectedEntity.GetComponent<Cataclysm::TransformComponent>();
			Cataclysm::Vesuvius::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}

		Cataclysm::Vesuvius::EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{
		CC_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistent = true;
		bool opt_fullscreen = opt_fullscreen_persistent;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_BrowserPanel->OnImGuiRender();

		ImGui::Begin("Stats");

#if 0
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<Cataclysm::TagComponent>().Tag;

		ImGui::Text("Hovered Entity: %s", name.c_str());
#endif

		auto stats = Cataclysm::Vesuvius::GetStats();
		ImGui::Text("Renderer 2D Stats: ");
		ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
		ImGui::Text("Quads:	   %d", stats.QuadCount);
		ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::Image((ImTextureID)s_Font->GetAtlasTexture()->GetRendererID(), { 512,512 }, { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
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
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<Cataclysm::TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Cataclysm::Input::IsKeyPressed(Cataclysm::Key::LeftControl) || Cataclysm::Input::IsKeyPressed(Cataclysm::Key::RightControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Cataclysm::Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

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

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Cataclysm::Ref<Cataclysm::Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;		//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}
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

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
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

		bool control = Cataclysm::Input::IsKeyPressed(Cataclysm::Key::LeftControl) || Cataclysm::Input::IsKeyPressed(Cataclysm::Key::RightControl);
		bool shift = Cataclysm::Input::IsKeyPressed(Cataclysm::Key::LeftShift) || Cataclysm::Input::IsKeyPressed(Cataclysm::Key::RightShift);
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
			CC_WARN("Could not load {0} - not a scene file", path.filename().string());
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
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
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
