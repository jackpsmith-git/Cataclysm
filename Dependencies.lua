IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Cataclysm/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Cataclysm/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Cataclysm/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Cataclysm/vendor/glm"
IncludeDir["stb"] = "%{wks.location}/Cataclysm/vendor/stb"
IncludeDir["entt"] = "%{wks.location}/Cataclysm/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Cataclysm/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Cataclysm/vendor/ImGuizmo"
IncludeDir["Box2D"] = "%{wks.location}/Cataclysm/vendor/Box2D/include"
IncludeDir["mono"] = "%{wks.location}/Cataclysm/vendor/mono/include"
IncludeDir["filewatch"] = "%{wks.location}/Cataclysm/vendor/filewatch"
IncludeDir["msdfgen"] = "%{wks.location}/Cataclysm/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Cataclysm/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Cataclysm/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

--  Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["Bcrypt"] = "Bcrypt.lib"

