project "Cataclysm"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	buildoptions{"/utf-8"}
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "ccpch.h"
	pchsource "src/ccpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb/**.h",
		"vendor/stb/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE",
        "GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.msdf_atlas_gen}"
	}

	links
	{
		"Box2D",
		"GLFW",
		"Glad",
		"ImGui",
		"msdf-atlas-gen",
		"yaml-cpp",
		"opengl32.lib",

		"%{Library.mono}"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
    systemversion "latest"
    
        defines
        {
        }

		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.Bcrypt}"
		}

	filter "configurations:Debug"
		defines "CC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CC_DIST"
		runtime "Release"
		optimize "on"
