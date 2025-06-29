local CataclysmRootDir = 'S:/repos/c++/Cataclysm'
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
