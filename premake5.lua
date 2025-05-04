include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Cataclysm"
	architecture "x86_64"
	startproject "Caterpillar"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	group "Dependencies"
		include "vendor/premake"
		include "Cataclysm/vendor/Box2D"
		include "Cataclysm/vendor/GLFW"
		include "Cataclysm/vendor/Glad"
		include "Cataclysm/vendor/msdf-atlas-gen"
		include "Cataclysm/vendor/imgui"
		include "Cataclysm/vendor/yaml-cpp"
	group ""

	group "Core"
		include "Cataclysm"
		include "CCML"
		include "Caterpillar"
	group ""