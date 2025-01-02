project "Premake"
    kind "Utility"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/**premake5.lua"
    }

    postbuildmessage "Regenerating project files!"
    postbuildcommands
    {
        "\"%{prj.location}bin/premake5\" %{_ACTION} --file=\"%{wks.location}premake5.lua\""
    }
