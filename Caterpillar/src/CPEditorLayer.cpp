#include "ccpch.h"
#include "CPEditorLayer.h"

#include "ImGui/CPImGui.h"
#include "CPStyles.h"
#include "CPUtils.h"

#include <cstdlib>

#ifdef CC_PLATFORM_WINDOWS
#include <Windows.h>
#include <ShObjIdl.h>
#endif

// ImGui
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "ImGuizmo.h"

// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Cataclysm
#include "Cataclysm/Scene/CCSceneSerializer.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/Audio/CCAudioEngine.h"
#include "Cataclysm/Math/CCMath.h"
#include "Cataclysm/Utils/CCPlatformUtils.h"
#include "Cataclysm/Asset/CCAssetSystem.h"

#include "Cataclysm/Scene/CCSceneEntityManager.h"

// Vesuvius
#include "Cataclysm/Vesuvius/VSFont.h"

namespace Cataclysm
{
	static CCRef<Vesuvius::VSFont> s_Font;
	static bool openNewProjectDialog = false;
	static char newProjectNameBuffer[128] = "";
	static std::string& newProjectDirectory = std::string();

	CPEditorLayer::CPEditorLayer()
		: CCLayer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		s_Font = Vesuvius::VSFont::GetDefault();
	}

	void CPEditorLayer::OnAttach()
	{
		CCScriptEngine::Init();
		CCAudioEngine::Init();
		CPResources::Initialize();

		Vesuvius::VSFramebufferSpecification fbSpec;
		fbSpec.Attachments = { Vesuvius::VSFramebufferTextureFormat::RGBA8, Vesuvius::VSFramebufferTextureFormat::RED_INTEGER, Vesuvius::VSFramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Vesuvius::VSFramebuffer::Create(fbSpec);

		m_EditorScene = CreateRef<CCScene>();
		m_ActiveScene = m_EditorScene;

		m_EditorCamera = Vesuvius::VSEditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Vesuvius::VSRenderer2D::SetLineWidth(4.0f);
	}

	void CPEditorLayer::OnDetach()
	{
		CCAudioEngine::Shutdown();
		CCScriptEngine::Shutdown();
	}

	void CPEditorLayer::ResizeScene()
	{
		if (Vesuvius::VSFramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}

	void CPEditorLayer::OnUpdate(Cataclysm::CCTimestep ts)
	{
		if (m_ProjectOpen)
		{
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			ResizeScene();

			Vesuvius::VSRenderer2D::ResetStats();
			m_Framebuffer->Bind();
			Vesuvius::VSRenderCommand::SetClearColor(glm::vec4(0, 0, 0, 0));
			Vesuvius::VSRenderCommand::Clear();

			m_Framebuffer->ClearAttachment(1, -1);

			switch (m_SceneState)
			{
				case CPSceneState::Edit:
				{
					if (m_ViewportFocused)
						m_CameraController.OnUpdate(ts);

					m_EditorCamera.OnUpdate(ts);
					m_ActiveScene->UpdateSceneEditor(m_EditorCamera);
					break;
				}
				case CPSceneState::Simulate:
				{
					m_EditorCamera.OnUpdate(ts);
					m_ActiveScene->UpdateSceneEditor(m_EditorCamera);
					break;
				}
				case CPSceneState::Play:
				{
					m_ActiveScene->UpdateSceneRuntime(ts, m_EditorCamera);
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
	}

	template <typename T>
	void RenderColliders(Cataclysm::CCRef<CCScene> activeScene)
	{
		static_assert(std::is_same<T, BoxCollider2D>::value || std::is_same<T, CircleCollider2D>::value, "T must be a collider!");

		auto view = CCSceneEntityManager::GetAllEntitiesWith<Transform, T>(activeScene.get());
		for (auto entity : view)
		{
			auto [tc, cc] = view.get<Transform, T>(entity);
			glm::mat4 transform = tc.GlobalTransform;

			glm::vec3 translation, rotation, scale;
			Cataclysm::Math::DecomposeTransform(transform, translation, rotation, scale);

			float cosAngle = cos(rotation.z);
			float sinAngle = sin(rotation.z);

			glm::vec2 rotatedOffset = {
				cc.Offset.x * cosAngle - cc.Offset.y * sinAngle,
				cc.Offset.x * sinAngle + cc.Offset.y * cosAngle
			};

			glm::vec3 colliderTranslation = translation + glm::vec3(rotatedOffset, 0.0f);
			glm::vec3 colliderScale = scale;

			constexpr bool isBoxCollider = std::is_same_v<T, BoxCollider2D>;
			constexpr bool isCircleCollider = std::is_same_v<T, CircleCollider2D>;

			if constexpr (isBoxCollider)
			{
				colliderScale = scale * glm::vec3(cc.Size * 2.0f, 1.0f);
			}
			else if constexpr (isCircleCollider)
			{
				float maxScale = glm::max(scale.x, scale.y);
				colliderScale = glm::vec3(cc.Radius * 2.0f * maxScale, cc.Radius * 2.0f * maxScale, 1.0f);
			}

			glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), colliderTranslation)
				* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1))
				* glm::scale(glm::mat4(1.0f), colliderScale);

			if constexpr (isBoxCollider)
			{
				Vesuvius::VSRenderer2D::DrawRect(colliderTransform, glm::vec4(0, 1, 0, 1));
			}
			else if constexpr (isCircleCollider)
			{
				Vesuvius::VSRenderer2D::DrawCircleDebug(colliderTransform, glm::vec4(0, 1, 0, 1), 0.05f);
			}
		}
	}

	void CPEditorLayer::OnOverlayRender()
	{
		if (m_SceneState == CPSceneState::Play)
		{
			Cataclysm::Entity camera = CCSceneEntityManager::GetPrimaryCameraEntity(m_ActiveScene.get());
			if (!camera)
				return;
			Vesuvius::VSRenderer2D::BeginScene(camera.GetComponent<Cataclysm::Camera>().SceneCamera, camera.GetComponent<Cataclysm::Transform>().GetTransform());
		}
		else
		{
			Vesuvius::VSRenderer2D::BeginScene(m_EditorCamera);

			if (m_ShowPhysicsColliders)
			{
				RenderColliders<BoxCollider2D>(m_ActiveScene);
				RenderColliders<CircleCollider2D>(m_ActiveScene);
			}

			if (Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
			{
				const Cataclysm::Transform& transform = selectedEntity.GetComponent<Cataclysm::Transform>();
				Vesuvius::VSRenderer2D::DrawRect(transform.GlobalTransform, glm::vec4(1.0f, 0.65f, 0.0f, 1.0f));
			}
		}


		Vesuvius::VSRenderer2D::EndScene();
	}

	void CPEditorLayer::OnImGuiRender(CCTimestep ts)
	{
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

		MenuBar();
		if (m_ProjectOpen)
		{
			UI_Toolbar();
		}

		StatusBar();
		if (m_ProjectOpen)
		{
			RenderPanels(ts);
			RenderScene();
		}
		else
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImDrawList* drawList = ImGui::GetForegroundDrawList(viewport);

			ImVec2 imageSize = ImVec2(400.0f * CPStyles::GetUIScale(), 400.0f * CPStyles::GetUIScale());
			ImVec2 screenCenter = ImVec2(
				viewport->Pos.x + viewport->Size.x * 0.5f,
				viewport->Pos.y + viewport->Size.y * 0.5f
			);

			ImVec2 topLeft = ImVec2(screenCenter.x - imageSize.x * 0.5f, screenCenter.y - imageSize.y * 0.5f);
			ImVec2 bottomRight = ImVec2(topLeft.x + imageSize.x, topLeft.y + imageSize.y);

			ImTextureID texID = (ImTextureID)(uint64_t)CPResources::ICON_CATACLYSM_WHITE->GetRendererID();
			drawList->AddImage(texID, topLeft, bottomRight, ImVec2(0, 1), ImVec2(1, 0), ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.1f)));
		}

		ImGui::End();

		if (m_ReloadLayout)
		{
			ResetLayout();
			m_ReloadLayout = false;
		}
	}

	void CPEditorLayer::RenderPanels(CCTimestep ts)
	{
		m_SceneHierarchyPanel.OnImGuiRender();
		m_BrowserPanel->OnImGuiRender();

		if (m_ShowFrameDebuggerPanel)
			m_FrameDebuggerPanel->OnImGuiRender(ts);

		if (m_ShowOutputPanel)
			m_OutputPanel->OnImGuiRender();

		if (m_ShowProfilerPanel)
			m_ProfilerPanel->OnImGuiRender();

		if (m_ShowSettingsPanel)
			m_SettingsPanel->OnImGuiRender(m_EditorScene);

		if (m_ShowVesuviusPanel)
			m_VesuviusPanel->OnImGuiRender();

		// m_TestPanel->OnImGuiRender();

		// if (m_ShowWelcomePanel)
		//	m_WelcomePanel->OnImGuiRender(m_ShowWelcomePanel);

		m_GamePanel->OnImGuiRender(m_ActiveScene, ts, static_cast<int>(m_SceneState));

	}

	void CPEditorLayer::RenderScene()
	{
		ImVec4 skyboxColor = ImVec4(m_ActiveScene->SkyboxColor.x, m_ActiveScene->SkyboxColor.y, m_ActiveScene->SkyboxColor.z, m_ActiveScene->SkyboxColor.w);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, skyboxColor);
		CPImGui::BeginPanel("Scene");
		ImGui::PopStyleColor();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Cataclysm::CCApplication::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		CPImGui::Image(m_Framebuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		RenderGizmos();

		CPImGui::EndPanel();
	}

	void CPEditorLayer::RenderGizmos()
	{
		Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			if (m_SceneState == CPSceneState::Edit)
			{
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				auto& tc = selectedEntity.GetComponent<Cataclysm::Transform>();

				CCUUID entityID = selectedEntity.GetUUID();

				CCUUID parentID = CCSceneEntityManager::GetParent(m_ActiveScene.get(), entityID);
				glm::mat4 parentTransform = glm::mat4(1.0f);
				if (parentID)
					parentTransform = CCSceneEntityManager::GetWorldTransform(m_ActiveScene.get(), parentID);

				glm::mat4 worldTransform = CCSceneEntityManager::GetWorldTransform(m_ActiveScene.get(), entityID);

				bool snap = Cataclysm::CCInput::IsKeyDown(Cataclysm::CCKey::LeftControl) || Cataclysm::CCInput::IsKeyDown(Cataclysm::CCKey::RightControl);
				float snapValue = 0.5f;

				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };
				ImGuizmo::Manipulate(
					glm::value_ptr(cameraView),
					glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::WORLD,
					glm::value_ptr(worldTransform),
					nullptr,
					snap ? snapValues : nullptr
				);

				if (ImGuizmo::IsUsing())
				{
					CCSceneEntityManager::SetWorldTransform(m_ActiveScene.get(), selectedEntity.GetUUID(), worldTransform);
				}
			}
		}
	}

	void CPEditorLayer::MenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (m_ProjectOpen)
				{
					if (ImGui::MenuItem("New Scene", "Ctrl+N"))
						NewScene();

					if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
						SaveScene();

					if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
						SaveSceneAs();

					ImGui::Separator();

					if (ImGui::MenuItem("New Project"))
					{
						CloseProject();
						memset(newProjectNameBuffer, 0, sizeof(newProjectNameBuffer));
						openNewProjectDialog = true;
					}

					if (ImGui::MenuItem("Open Project", "Ctrl+O"))
					{
						CloseProject();
						if (OpenProject())
							m_ProjectOpen = true;
					}

					if (ImGui::MenuItem("Save Project", "Ctrl+Alt+S"))
					{
						SaveScene();
						SaveProject();
					}

					if (ImGui::MenuItem("Close Project", "Ctrl+Shift+X"))
					{
						CloseProject();
					}
				}
				else
				{
					if (ImGui::MenuItem("New Project"))
					{
						memset(newProjectNameBuffer, 0, sizeof(newProjectNameBuffer));
						openNewProjectDialog = true;
					}

					if (ImGui::MenuItem("Open Project", "Ctrl+O"))
					{
						if (OpenProject())
							m_ProjectOpen = true;
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Cataclysm::CCApplication::Get().Close();

				ImGui::EndMenu();
			}

			if (m_ProjectOpen)
			{
				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::BeginMenu("Panels"))
					{
						ImGui::MenuItem("Frame", nullptr, &m_ShowFrameDebuggerPanel);
						ImGui::MenuItem("Output", nullptr, &m_ShowOutputPanel);
						ImGui::MenuItem("Profiler", nullptr, &m_ShowProfilerPanel);
						ImGui::MenuItem("Settings", nullptr, &m_ShowSettingsPanel);
						ImGui::MenuItem("Vesuvius", nullptr, &m_ShowVesuviusPanel);

						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Reset Layout", "Ctrl+L"))
					{
						m_ReloadLayout = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Project"))
				{
					if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
						CCScriptEngine::ReloadAssembly();

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Scene"))
				{
					ImGui::MenuItem("Show Physics Colliders", nullptr, &m_ShowPhysicsColliders);

					ImGui::Separator();
					if (ImGui::MenuItem("Reset Editor Camera"))
						m_EditorCamera.Reset();

					ImGui::EndMenu();
				}
			}

			if (ImGui::BeginMenu("Help"))
			{	
				if (CPImGui::MenuItem("Online Documentation", CPResources::ICON_DOCUMENTATION, "Ctrl+H"))
				{
					const std::string url = "https://jackpsmith.com/cataclysm/docs/";
					std::string command = "start " + url;
					system(command.c_str());
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (openNewProjectDialog)
		{
			ImGui::OpenPopup("New Project");
			openNewProjectDialog = false;
		}

		if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (CPImGui::ImageButton(CPResources::ICON_DIRECTORY, CPStyles::GetIconSize()))
			{
#ifdef CC_PLATFORM_WINDOWS
				newProjectDirectory = OpenFolderDialog();
#endif
			}

			ImGui::SameLine();
			if (newProjectDirectory.empty())
			{
				ImGui::Text(newProjectDirectory.c_str());
			}
			else
			{
				std::string newProjectDirectoryWithTrailingBackslash = newProjectDirectory + "\\";
				ImGui::Text(newProjectDirectoryWithTrailingBackslash.c_str());
			}

			ImGui::Text("Project Name");
			ImGui::SameLine();
			ImGui::InputText("##Project Name", newProjectNameBuffer, IM_ARRAYSIZE(newProjectNameBuffer));

			if (ImGui::Button("Create Project", CPStyles::GetButtonSizeDialog()))
			{
				CreateProject(newProjectDirectory, newProjectNameBuffer);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", CPStyles::GetButtonSizeDialog()))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void CPEditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
		
		ImGui::GetStyle().WindowMinSize.y = CPStyles::GetPanelMenuHeight();
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);

		bool toolbarEnabled = (bool)m_ActiveScene;
		ImVec4 tintColor = CPStyles::ICON_COLOR_EDITOR_TOOLBAR;
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight();
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == CPSceneState::Edit || m_SceneState == CPSceneState::Play;
		bool hasPauseButton = m_SceneState != CPSceneState::Edit;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			if (hasPlayButton)
			{
				Cataclysm::CCRef<Vesuvius::VSTexture2D> icon = (m_SceneState == CPSceneState::Edit || m_SceneState == CPSceneState::Simulate) ? CPResources::ICON_PLAY : CPResources::ICON_STOP;
				if (CPImGui::ImageButton(icon, size, tintColor) && toolbarEnabled)
				{
					if (m_SceneState == CPSceneState::Edit || m_SceneState == CPSceneState::Simulate)
						OnScenePlay();
					else if (m_SceneState == CPSceneState::Play)
						OnSceneStop();
				}
			}

			if (hasPauseButton)
			{
				bool isPaused = m_ActiveScene->IsPaused();
				ImGui::SameLine();
				{
					CCRef<Vesuvius::VSTexture2D> icon = CPResources::ICON_PAUSE;
					if (CPImGui::ImageButton(icon, size, tintColor) && toolbarEnabled)
					{
						m_ActiveScene->SetPaused(!isPaused);
					}
				}

				if (isPaused)
				{
					ImGui::SameLine();
					{
						CCRef<Vesuvius::VSTexture2D> icon = CPResources::ICON_STEP;
						bool isPaused = m_ActiveScene->IsPaused();
						if (CPImGui::ImageButton(icon, size, tintColor) && toolbarEnabled)
						{
							m_ActiveScene->Step();
						}
					}
				}
			}

		ImGui::PopStyleVar();

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ((size * 2.315) + ((size + 3) * 3) + (size * 2) -3));

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 0));

			if (CPImGui::ImageButton(CPResources::ICON_RESET_EDITOR_CAMERA, ImVec2(size * 2.315, size), tintColor) && toolbarEnabled)
			{
				m_EditorCamera.Reset();
			}
			ImGui::SameLine();

			if (m_ShowPhysicsColliders)
			{
				if (CPImGui::ImageButton(CPResources::ICON_SHOW_PHYSICS_COLLIDERS, size, tintColor) && toolbarEnabled)
				{
					m_ShowPhysicsColliders = !(m_ShowPhysicsColliders);
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_SHOW_PHYSICS_COLLIDERS_FADED, size,  tintColor) && toolbarEnabled)
				{
					m_ShowPhysicsColliders = !(m_ShowPhysicsColliders);
				}
			}


			ImGui::SameLine();
			if (m_GizmoType == -1)
			{
				if (CPImGui::ImageButton(CPResources::ICON_EMPTY, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = -1;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_EMPTY_FADED, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = -1;
				}
			}

			ImGui::PopStyleVar();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		
			ImGui::SameLine();
			if (m_GizmoType == 0)
			{
				if (CPImGui::ImageButton(CPResources::ICON_TRANSLATE, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = 0;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_TRANSLATE_FADED, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = 0;
				}
			}

			ImGui::SameLine();
			if (m_GizmoType == 1)
			{
				if (CPImGui::ImageButton(CPResources::ICON_ROTATE, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = 1;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_ROTATE_FADED, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = 1;
				}
			}

			ImGui::SameLine();
			if (m_GizmoType == 2)
			{
				if (CPImGui::ImageButton(CPResources::ICON_SCALE, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = 2;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_SCALE_FADED, size, tintColor) && toolbarEnabled)
				{
					m_GizmoType = 2;
				}
			}

		ImGui::PopStyleVar(4);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void CPEditorLayer::StatusBar()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		float barHeight = CPStyles::GetStatusBarHeight();

		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - barHeight));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, barHeight));
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 255));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 100, 100, 255));

		ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionMax().x, 0), ImGuiCond_Always);
		ImGui::Begin("##statusbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

		ImGui::Spacing();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		CPImGui::Image(CPResources::ICON_CATACLYSM, barHeight);

		ImGui::SameLine();
		ImGui::Spacing();

		ImGui::SameLine();
		ImGui::Text("Cataclysm v2.0.0 - Copyright 2025 Jack P Smith");
		ImGui::SameLine();

		ImGui::Spacing();
		ImGui::SameLine();

		float scaleButtonWidth = 50.0f * CPStyles::GetUIScale();
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ((barHeight * 2) + scaleButtonWidth));

		std::stringstream ss;
		ss << std::fixed << std::setprecision(0) << (CPStyles::GetUIScale() * 100.0f) << "%";
		std::string label = ss.str();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		CPImGui::Image(CPResources::ICON_UI_SCALE, barHeight);
		ImGui::SameLine();

		if (ImGui::Button(label.c_str(), ImVec2(scaleButtonWidth, CPStyles::GetStatusBarHeight())))
		{
			ImGui::OpenPopup("ScalePopup");
		}

		if (ImGui::BeginPopup("ScalePopup"))
		{
			if (ImGui::MenuItem("20%"))  { CPStyles::SetUIScale(0.20f); }
			if (ImGui::MenuItem("50%"))  { CPStyles::SetUIScale(0.50f); }
			if (ImGui::MenuItem("75%"))  { CPStyles::SetUIScale(0.75f); }
			if (ImGui::MenuItem("90%"))  { CPStyles::SetUIScale(0.90f); }
			if (ImGui::MenuItem("100%")) { CPStyles::SetUIScale(1.00f); }
			if (ImGui::MenuItem("125%")) { CPStyles::SetUIScale(1.25f); }
			if (ImGui::MenuItem("150%")) { CPStyles::SetUIScale(1.50f); }
			if (ImGui::MenuItem("175%")) { CPStyles::SetUIScale(1.75f); }
			if (ImGui::MenuItem("200%")) { CPStyles::SetUIScale(2.00f); }
			if (ImGui::MenuItem("250%")) { CPStyles::SetUIScale(2.50f); }
			if (ImGui::MenuItem("400%")) { CPStyles::SetUIScale(4.00f); }

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (CPImGui::ImageButton(CPResources::ICON_DOCUMENTATION, CPStyles::GetStatusBarHeight()))
		{
			const std::string url = "https://jackpsmith.com/cataclysm/docs/";
			std::string command = "start " + url;
			system(command.c_str());
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		ImGui::End();
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(4);
	}

	void CPEditorLayer::OnScenePlay()
	{
		ImGui::SetWindowFocus("Game");
		if (m_SceneState == CPSceneState::Simulate)
			OnSceneStop();

		m_SceneState = CPSceneState::Play;
		m_ActiveScene = Cataclysm::CCScene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void CPEditorLayer::OnScenePause()
	{
		if (m_SceneState == CPSceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void CPEditorLayer::OnSceneStop()
	{
		CC_CORE_ASSERT("Unknown scene state!", m_SceneState == CPSceneState::Play || m_SceneState == CPSceneState::Simulate);
		if (m_SceneState == CPSceneState::Play)
			m_ActiveScene->OnRuntimeStop();

		m_SceneState = CPSceneState::Edit;
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void CPEditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != CPSceneState::Edit)
			return;

		Cataclysm::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = CCSceneEntityManager::DuplicateEntity(m_EditorScene.get(), selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}

	void CPEditorLayer::OnEvent(Cataclysm::CCEvent& e)
	{
		m_CameraController.OnEvent(e);

		if (m_SceneState == CPSceneState::Edit)
			m_EditorCamera.OnEvent(e);

		CCEventDispatcher dispatcher(e);
		dispatcher.Dispatch<CCKeyPressedEvent>(CC_BIND_EVENT_FN(CPEditorLayer::OnKeyPressed));
		dispatcher.Dispatch<CCMouseButtonPressedEvent>(CC_BIND_EVENT_FN(CPEditorLayer::OnMouseButtonPressed));
	}

	bool CPEditorLayer::OnKeyPressed(CCKeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool control = CCInput::IsKeyDown(CCKey::LeftControl) || CCInput::IsKeyDown(CCKey::RightControl);
		bool shift = CCInput::IsKeyDown(CCKey::LeftShift) || CCInput::IsKeyDown(CCKey::RightShift);
		bool alt = CCInput::IsKeyDown(CCKey::LeftAlt) || CCInput::IsKeyDown(CCKey::RightAlt);
		switch (e.GetKeyCode())
		{
			case CCKey::N:
			{
				if (control)
					NewScene();

				break;
			}
			case CCKey::O:
			{
				if (!m_ProjectOpen)
				{
					if (control)
						OpenProject();
				}

				break;
			}
			case CCKey::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else if (alt)
					{
						SaveScene();
						SaveProject();
					}
					else
						SaveScene();
				}

				break;
			}
			case CCKey::X:
			{
				if (control && shift)
				{
					CloseProject();
				}

				break;
			}
			case CCKey::L:
			{
				if (control)
				{
					m_ReloadLayout = true;
				}

				break;
			}
			case CCKey::H:
			{
				if (control)
				{
					const std::string url = "https://jackpsmith.com/cataclysm/docs/";
					std::string command = "start " + url;
					system(command.c_str());
				}

				break;
			}

			// Scene Commands
			case CCKey::D:
			{
				if (control)
					OnDuplicateEntity();

				break;
			}

			// Gizmos
			case CCKey::Q:
			{
				m_GizmoType = -1;
				break;
			}
			case CCKey::W:
			{
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case CCKey::E:
			{
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case CCKey::R:
			{
				if (control)
				{
					CCScriptEngine::ReloadAssembly();
				}
				else
				{
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
			case CCKey::Delete:
			{
				if (CCApplication::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
				{
					Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntity)
					{
						m_SceneHierarchyPanel.SetSelectedEntity({});
						CCSceneEntityManager::DestroyEntity(m_ActiveScene.get(), selectedEntity);
					}
				}
				break;
			}
		}

		return false;
	}

	bool CPEditorLayer::OnMouseButtonPressed(CCMouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == CCMouse::ButtonLeft)
		{
			if (m_ViewportHovered && m_ViewportFocused && !ImGuizmo::IsOver())
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void CPEditorLayer::CreateProject(std::string directory, std::string projectName)
	{
		// Create directories
		std::filesystem::path projectRoot = std::filesystem::path(directory) / projectName;

		if (std::filesystem::exists(projectRoot))
		{
			CC_CORE_ERROR("Project already exists!");
			return;
		}

		std::filesystem::path assetsDirectory = projectRoot / "Assets";
		std::filesystem::path audioDirectory = assetsDirectory / "Audio";
		std::filesystem::path fontsDirectory = assetsDirectory / "Fonts";
		std::filesystem::path scenesDirectory = assetsDirectory / "Scenes";
		std::filesystem::path texturesDirectory = assetsDirectory / "Textures";
		std::filesystem::path scriptsDirectory = assetsDirectory / "Scripts";
		std::filesystem::path sourceDirectory = scriptsDirectory / "Source";
		std::filesystem::create_directories(audioDirectory);
		std::filesystem::create_directories(fontsDirectory);
		std::filesystem::create_directories(scenesDirectory);
		std::filesystem::create_directories(texturesDirectory);
		std::filesystem::create_directories(sourceDirectory);

		// Create premake file
		std::filesystem::path premakeTargetDirectory = scriptsDirectory / "premake5.lua";

		std::string premakeScript = R"(local CataclysmRootDir = 'S:/repos/c++/Cataclysm'
include (CataclysmRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "NewProject"
	architecture "x86_64"
	startproject "NewProject"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NewProject"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	links
	{
		"CCML"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "Cataclysm"
	include (CataclysmRootDir .. "/CCML")
group ""
)";
		std::string oldName = "NewProject";
		size_t pos = 0;
		while ((pos = premakeScript.find(oldName, pos)) != std::string::npos)
		{
			premakeScript.replace(pos, oldName.length(), projectName);
			pos += projectName.length();
		}

		std::ofstream premakeOut(premakeTargetDirectory, std::ios::out | std::ios::trunc);
		if (premakeOut)
		{
			premakeOut << premakeScript;
			premakeOut.close();
		}
		else
		{
			CC_CORE_ERROR("Failed to generate premake file!");
			return;
		}

		// Create batch file
		std::filesystem::path batchTargetDirectory = scriptsDirectory / (projectName + ".bat");
		std::string batchScript = R"(@echo off
call S:\repos\c++\Cataclysm\vendor\premake\bin\premake5.exe vs2022
)";

		std::ofstream batchOut(batchTargetDirectory, std::ios::out | std::ios::trunc);
		if (batchOut)
		{
			batchOut << batchScript;
			batchOut.close();
		}
		else
		{
			CC_CORE_ERROR("Failed to generate batch file!");
			return;
		}

		// Run batch file
		CPFileSystemUtils::RunBatch(batchTargetDirectory);

		// Create default scene file
		std::filesystem::path defaultScenePath = scenesDirectory / "NewScene.ccscene";
		std::ofstream ccscene(defaultScenePath);
		ccscene << "Scene: New Scene" << "\n";
		ccscene << "SceneSettings:" << "\n";
		ccscene << "  SkyboxColor: [0, 0, 0, 1]" << "\n";
		ccscene << "  Entities: " << "\n";
		ccscene.close();

		// Create project file
		std::filesystem::path newCCprojPath = projectRoot / (projectName + ".ccproj");
		std::ofstream ccproj(newCCprojPath);
		ccproj << "Project:\n";
		ccproj << "  Name: " << projectName << "\n";
		ccproj << "  StartScene: Scenes/NewScene.ccscene" << "\n";
		ccproj << "  AssetDirectory: Assets" << "\n";
		ccproj << "  ScriptModulePath: Scripts/Binaries/" << projectName << ".dll" << "\n";
		ccproj << "  PixelDensity: " << "100" << "\n";
		ccproj.close();

		// Create build batch file
		std::filesystem::path buildTargetDirectory = scriptsDirectory / "build.bat";
		std::string buildScript = R"(@echo off
setlocal

for /f "usebackq tokens=*" %%i in (`"S:\repos\c++\Cataclysm\vendor\vswhere\bin\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
    set "MSBUILD=%%i\MSBuild\Current\Bin\MSBuild.exe"
)

if not exist "%MSBUILD%" (
    echo MSBuild not found! Please ensure Visual Studio with MSBuild is installed.
    exit /b 1
)

"%MSBUILD%" "%~dp0NewProject.sln" /p:Configuration=Release /m
if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo Build succeeded!
endlocal)";

		size_t cpos = 0;
		while ((cpos = buildScript.find(oldName, cpos)) != std::string::npos)
		{
			buildScript.replace(cpos, oldName.length(), projectName);
			cpos += projectName.length();
		}

		std::ofstream buildOut(buildTargetDirectory, std::ios::out | std::ios::trunc);
		if (buildOut)
		{
			buildOut << buildScript;
			buildOut.close();
		}
		else
		{
			CC_CORE_ERROR("Failed to generate build file!");
			return;
		}

		// Run build batch file
		CPFileSystemUtils::RunBatch(buildTargetDirectory);

		// Open project
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		OpenProject(newCCprojPath);
		m_ProjectOpen = true;
	}

	bool CPEditorLayer::OpenProject()
	{
		std::string filepath = CCFileDialogs::OpenFile("Cataclysm Project (*.ccproj)\0*.ccproj\0", ".ccproj");

		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void CPEditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (CCProject::Load(path))
		{
			CCScriptEngine::LoadProjectScripts();
			CCAssetSystem::ImportAssets(CCProject::GetAssetDirectory());

			auto startScenePath = CCProject::GetAssetFileSystemPath(CCProject::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			m_BrowserPanel = CreateScope<CPBrowserPanel>();
			m_FrameDebuggerPanel = CreateScope<CPFrameDebuggerPanel>();
			m_OutputPanel = CreateScope<CPOutputPanel>();
			m_FrameDebuggerPanel = CreateScope<CPFrameDebuggerPanel>();

			m_GamePanel = CreateScope<CPGamePanel>();
			m_GamePanel->Init();

			m_ProfilerPanel = CreateScope<CPProfilerPanel>();
			m_ProfilerPanel->Init();

			m_SettingsPanel = CreateScope<CPSettingsPanel>();
			m_VesuviusPanel = CreateScope<CPVesuviusPanel>();

			m_TestPanel = CreateScope<CPTestPanel>();
		}
		else
		{
			CC_CORE_ERROR("[CPEditorLayer::OpenProject] Failed to open project at path: {0}", path.string());
			return;
		}
	}

	void CPEditorLayer::SaveProject()
	{
		std::filesystem::path projectDir = CCProject::GetProjectDirectory();
		std::string& projectName = CCProject::GetActive()->GetConfig().Name;
		std::filesystem::path filePath = projectDir / (projectName + ".ccproj");
		CCProject::SaveActive(filePath);
	}

	void CPEditorLayer::CloseProject()
	{
		CCApplication::Get().SetWindowTitle("Caterpillar");
		m_ProjectOpen = false;
		m_EditorScene = nullptr;
		m_ActiveScene = nullptr;
		CCScriptEngine::UnloadProjectScripts();
		CCAssetSystem::Shutdown();
	}

	void CPEditorLayer::NewScene()
	{
		m_ActiveScene = Cataclysm::CreateRef<Cataclysm::CCScene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_EditorScenePath = std::filesystem::path();
	}

	void CPEditorLayer::OpenScene()
	{
		std::string filepath = Cataclysm::CCFileDialogs::OpenFile("Cataclysm Scene (*.ccscene)\0*.ccscene\0", ".ccscene");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void CPEditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != CPSceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".ccscene")
		{
			CC_WARN("[CPEditorLayer::OpenScene] Could not load '{0}' - not a scene file", path.filename().string());
			return;
		}

		Cataclysm::CCRef<Cataclysm::CCScene> newScene = Cataclysm::CreateRef<Cataclysm::CCScene>();
		Cataclysm::CCSceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_SceneHierarchyPanel.SetContext(m_EditorScene);

			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}

	void CPEditorLayer::SaveScene()
	{
		if (m_SceneState == CPSceneState::Edit)
		{
			if (!m_EditorScenePath.empty())
				CPSceneUtils::SerializeScene(m_ActiveScene, m_EditorScenePath);
			else
				SaveSceneAs();
		}
		else if (m_SceneState == CPSceneState::Play)
		{
			CC_CORE_WARN("Cannot save scene while in Play Mode!");
		}
		else if (m_SceneState == CPSceneState::Simulate)
		{
			CC_CORE_WARN("Cannot save scene while in Simulate Mode!");
		}
	}

	void CPEditorLayer::SaveSceneAs()
	{
		std::string filepath = Cataclysm::CCFileDialogs::SaveFile("Cataclysm Scene (*.ccscene)\0*.ccscene\0", ".ccscene");
		if (!filepath.empty())
		{
			CPSceneUtils::SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void CPEditorLayer::ResetLayout()
	{
		std::string activeini(ImGui::GetIO().IniFilename);
		std::string content = R"([Window][Dockspace Demo]
Pos=0,0
Size=1600,900
Collapsed=0

[Window][Debug##Default]
Pos=60,11
Size=400,400
Collapsed=0

[Window][Settings]
Pos=1215,22
Size=385,565
Collapsed=0
DockId=0x00000015,1

[Window][Viewport]
Pos=265,51
Size=200,451
Collapsed=0
DockId=0x00000019,0

[Window][Hierarchy]
Pos=0,22
Size=212,523
Collapsed=0
DockId=0x00000003,0

[Window][Properties]
Pos=1309,22
Size=291,664
Collapsed=0
DockId=0x00000015,0

[Window][Stats]
Pos=1283,727
Size=317,173
Collapsed=0
DockId=0x00000006,0

[Window][Content Browser]
Pos=0,653
Size=1245,247
Collapsed=0
DockId=0x0000000A,0

[Window][Browser]
Pos=0,614
Size=1225,286
Collapsed=0
DockId=0x00000017,0

[Window][##toolbar]
Pos=214,22
Size=1093,20
Collapsed=0
DockId=0x0000000E,0

[Window][Vesuvius]
Pos=1309,688
Size=291,212
Collapsed=0
DockId=0x00000016,1

[Window][Frame Debugger]
Pos=1183,472
Size=417,428
Collapsed=0
DockId=0x00000005,1

[Window][Dear ImGui Demo]
ViewportPos=890,132
ViewportId=0xE927CF2F
Size=550,903
Collapsed=0

[Window][Output]
Pos=747,547
Size=560,353
Collapsed=0
DockId=0x00000018,0

[Window][Project]
Pos=0,547
Size=745,353
Collapsed=0
DockId=0x00000017,0

[Window][##statusbar]
Pos=0,876
Size=1600,26
Collapsed=0

[Window][Cataclysm v1.0.0   2025 Jack P Smith]
Pos=0,875
Size=1600,25
Collapsed=0
DockId=0x00000012,0

[Window][Cataclysm v1.0.0 - Copyright 2025 Jack P Smith]
Pos=0,875
Size=1600,25
Collapsed=0
DockId=0x00000014,0

[Window][Game]
Pos=747,44
Size=560,501
Collapsed=0
DockId=0x0000001A,0

[Window][Scene]
Pos=214,44
Size=531,501
Collapsed=0
DockId=0x00000019,0

[Window][Frame]
Pos=1309,688
Size=291,212
Collapsed=0
DockId=0x00000016,0

[Window][Documentation]
Pos=321,51
Size=910,448
Collapsed=0
DockId=0x00000019,2

[Window][Profiler]
Pos=640,562
Size=610,338
Collapsed=0
DockId=0x00000017,1

[Window][Welcome]
Pos=321,51
Size=910,501
Collapsed=0
DockId=0x00000019,2

[Window][Create New Project]
Pos=131,111
Size=408,150
Collapsed=0

[Window][New Project]
Pos=569,332
Size=342,92
Collapsed=0

[Window][New Folder]
Pos=445,380
Size=746,86
Collapsed=0

[Window][New Script]
Pos=640,407
Size=791,86
Collapsed=0

[Table][0x47600645,3]
RefScale=18
Column 0  Width=55
Column 1  Width=55
Column 2  Weight=1.0000

[Table][0xDE6957FF,6]
RefScale=18
Column 0  Width=55
Column 1  Width=55
Column 2  Width=-1
Column 3  Weight=1.0000
Column 4  Weight=1.0000
Column 5  Weight=-1.0000

[Table][0x20DF7A63,4]
RefScale=18
Column 0  Width=31 Sort=0v
Column 1  Width=52
Column 2  Width=42
Column 3  Weight=1.0000

[Table][0xFED40537,3]
Column 0  Weight=0.9578
Column 1  Weight=1.0389
Column 2  Weight=1.0034

[Docking][Data]
DockSpace                   ID=0x6DCE998E Window=0x091AB4BE Pos=86,131 Size=1600,878 Split=Y
  DockNode                  ID=0x00000013 Parent=0x6DCE998E SizeRef=1600,849 Split=Y
    DockNode                ID=0x00000011 Parent=0x00000013 SizeRef=1600,849 Split=Y
      DockNode              ID=0x00000001 Parent=0x00000011 SizeRef=1600,750 Split=X
        DockNode            ID=0x00000007 Parent=0x00000001 SizeRef=1307,876 Split=Y
          DockNode          ID=0x00000009 Parent=0x00000007 SizeRef=1600,627 Split=Y
            DockNode        ID=0x0000000B Parent=0x00000009 SizeRef=892,523 Split=X Selected=0x13926F0B
              DockNode      ID=0x00000003 Parent=0x0000000B SizeRef=212,553 Selected=0x29EABFBD
              DockNode      ID=0x00000004 Parent=0x0000000B SizeRef=1093,553 Split=Y Selected=0x13926F0B
                DockNode    ID=0x0000000E Parent=0x00000004 SizeRef=893,20 HiddenTabBar=1 Selected=0xF0F70764
                DockNode    ID=0x0000000F Parent=0x00000004 SizeRef=893,501 Split=X Selected=0xE192E354
                  DockNode  ID=0x00000019 Parent=0x0000000F SizeRef=531,621 CentralNode=1 Selected=0xE192E354
                  DockNode  ID=0x0000001A Parent=0x0000000F SizeRef=560,621 Selected=0x26816F31
            DockNode        ID=0x0000000C Parent=0x00000009 SizeRef=892,353 Split=X Selected=0xD04A4B96
              DockNode      ID=0x00000017 Parent=0x0000000C SizeRef=745,239 Selected=0xD04A4B96
              DockNode      ID=0x00000018 Parent=0x0000000C SizeRef=560,239 Selected=0x0D42FFB9
          DockNode          ID=0x0000000A Parent=0x00000007 SizeRef=1600,247 Selected=0xBF096F38
        DockNode            ID=0x00000008 Parent=0x00000001 SizeRef=291,876 Split=Y Selected=0x199AB496
          DockNode          ID=0x00000005 Parent=0x00000008 SizeRef=353,701 Split=Y Selected=0x199AB496
            DockNode        ID=0x00000015 Parent=0x00000005 SizeRef=385,557 Selected=0x199AB496
            DockNode        ID=0x00000016 Parent=0x00000005 SizeRef=385,178 Selected=0x1754F4F0
          DockNode          ID=0x00000006 Parent=0x00000008 SizeRef=353,173 Selected=0x2E4EBCAA
      DockNode              ID=0x00000002 Parent=0x00000011 SizeRef=1600,124 Selected=0xDD1E36E9
    DockNode                ID=0x00000012 Parent=0x00000013 SizeRef=1600,25 Selected=0x53742E4A
  DockNode                  ID=0x00000014 Parent=0x6DCE998E SizeRef=1600,25 Selected=0x8FD54B92


			)";
		
			std::ofstream outFile(activeini);
			if (outFile.is_open())
			{
				outFile << content;
				outFile.close();
			}
			else
			{
				CC_CORE_ERROR("Failed to open Caterpillar.ini!");
			}

			ImGui::LoadIniSettingsFromDisk(ImGui::GetIO().IniFilename);

			m_ShowFrameDebuggerPanel = false;
			m_ShowOutputPanel = true;
			m_ShowProfilerPanel = false;
			m_ShowSettingsPanel = false;
			m_ShowVesuviusPanel = false;
			m_ShowWelcomePanel = false;

			CPStyles::SetUIScale(1.0f);
	}

	std::string CPEditorLayer::OpenFolderDialog(HWND owner)
	{
		std::string result;

		IFileDialog* pFileDialog = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

		if (SUCCEEDED(hr))
		{
			DWORD options;
			pFileDialog->GetOptions(&options);
			pFileDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

			hr = pFileDialog->Show(owner);
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem = nullptr;
				if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
				{
					PWSTR path = nullptr;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &path)))
					{
						char buffer[MAX_PATH];
						WideCharToMultiByte(CP_UTF8, 0, path, -1, buffer, MAX_PATH, nullptr, nullptr);
						result = buffer;
						CoTaskMemFree(path);
					}
					pItem->Release();
				}
			}
			pFileDialog->Release();
		}

		return result;
	}
}
