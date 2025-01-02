project "Caterpillar"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	buildoptions{"/utf-8"}
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Cataclysm/vendor/spdlog/include",
		"%{wks.location}/Cataclysm/src",
		"%{wks.location}/Cataclysm/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Cataclysm"
	}

	filter "system:windows"
		systemversion "latest"
        
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

