#include "ccpch.h"
#include "Cataclysm/ImGui/CCImGuiLayer.h"

#include "Cataclysm/Core/CCApplication.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Cataclysm/ImGui/OpenSans-Light-fontdata.h"
#include "Cataclysm/ImGui/OpenSans-Bold-fontdata.h"
#include "Cataclysm/ImGui/IBMPlexMono-Light-fontdata.h"
#include "Cataclysm/ImGui/IBMPlexMono-Regular-fontdata.h"
#include "Cataclysm/ImGui/IBMPlexMono-SemiBold-fontdata.h"
#include "Cataclysm/ImGui/IBMPlexMono-Bold-fontdata.h"

#include <GLFW/glfw3.h>

#include <filesystem>

#include "ImGuizmo.h"
#include <ShlObj.h>

namespace Cataclysm
{
	CCImGuiLayer::CCImGuiLayer()
		: CCLayer("ImGuiLayer")
	{

	}

	CCImGuiLayer::~CCImGuiLayer()
	{

	}

	void CCImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable multi-viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		float fontSize = 16.0f;// *2.0f;

		ImFontConfig config;
		config.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF((void*)OpenSans_Light_ttf, OpenSans_Light_ttf_len, fontSize, &config);
		io.Fonts->AddFontFromMemoryTTF((void*)OpenSans_Bold_ttf, OpenSans_Bold_ttf_len, fontSize, &config);

		io.Fonts->AddFontFromMemoryTTF((void*)IBMPlexMono_Light_ttf, IBMPlexMono_Light_ttf_len, fontSize, &config);
		io.Fonts->AddFontFromMemoryTTF((void*)IBMPlexMono_SemiBold_ttf, IBMPlexMono_SemiBold_ttf_len, fontSize, &config);
		io.Fonts->AddFontFromMemoryTTF((void*)IBMPlexMono_Bold_ttf, IBMPlexMono_Bold_ttf_len, fontSize, &config);
		io.FontDefault = io.Fonts->AddFontFromMemoryTTF((void*)IBMPlexMono_Regular_ttf, IBMPlexMono_Regular_ttf_len, fontSize, &config);

		// IMGUI INI
		PWSTR appDataPath = nullptr;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &appDataPath);
		std::filesystem::path imguiIniPath = appDataPath;
		CoTaskMemFree(appDataPath);
		imguiIniPath /= "Cataclysm/Caterpillar/Layouts";
		std::filesystem::create_directories(imguiIniPath);
		imguiIniPath /= "Caterpillar.ini";
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

[Window][CCML Documentation]
Pos=0,562
Size=679,338
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

		if (!std::filesystem::exists(imguiIniPath))
		{
			std::ofstream out(imguiIniPath);
			out << content;
			out.close();
		}

		ImGui::GetIO().IniFilename = _strdup(imguiIniPath.string().c_str());
		
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			style.TabRounding = 2.0f;
			style.TabBorderSize = 0.0f;
		}

		SetDarkThemeColors();

		CCApplication& app = CCApplication::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void CCImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void CCImGuiLayer::OnEvent(CCEvent& event)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void CCImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void CCImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		CCApplication& app = CCApplication::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void CCImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	uint32_t CCImGuiLayer::GetActiveWidgetID() const
	{
		return GImGui->ActiveId;
	}
}
