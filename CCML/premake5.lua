project "CCML"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	buildoptions{"/utf-8"}

	targetdir ("../Caterpillar/Resources/Scripts")
	objdir ("../Caterpillar/Resources/Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.cs"
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
