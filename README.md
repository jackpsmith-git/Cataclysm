# Cataclysm
<br />

<div align = "center">
    <a href="https://github.com/jackpsmith-git/Cataclysm">
    <img src="Resources/CataclysmWordmark.png">
</a>

Cataclysm is a cross-platform 2D rendering and game development engine written in C++. Included
with the Cataclysm Engine is Caterpillar, the visual editor for Cataclysm, as well as the Cataclysm
Core Mono Library (CCML);

The default renderer included with Cataclysm is Vesuvius, a 2D renderer that currently supports OpenGL.

Cataclysm supports C# scripting with Mono, an open-source implementation of .NET, as well as native
scripting in C++.
</div>

<details>
    <summary>Table of Contents</summary>
    <ol>
        <li><a href="#getting-started">Gettting Started</a></li>
            <ul>
            <li><a href="#dependencies">Dependencies</a></li>
            </ul>
        <li><a href="#compilation">Compilation</a></li>
        <li><a href="#changelog">Changelog</a></li>
        <li><a href="#documentation">Documentation</a></li>
        <li><a href="#license">License</a></li>
        <li><a href="#contributors">Contributors</a></li>
    </ol>
</details>

## Getting Started

Compiled Cataclysm binaries will be available soon. In the meantime, you can compile Cataclysm from source by following the instructions under <a href="#compilation">Compilation</a>.

Cataclysm is still early in development and will have bugs in distribution builds. Please help contribute to Cataclysm by reporting bugs and suggesting new features!

### Dependencies

Cataclysm includes all project dependencies. Cataclysm includes the following libraries:
<ul>
    <li><a href="https://github.com/erincatto/box2d">Box2D - 2D Physics Library</a></li>
    <li><a href="https://github.com/skypjack/entt">entt - ECS Library</a></li>
    <li><a href="https://github.com/ThomasMonkman/filewatch">FileWatch - File Watcher</a></li>
    <li><a href="https://github.com/Dav1dde/glad">glad - OpenGL implementation</a></li>
    <li><a href="https://www.glfw.org/">glfw - Window & Events Library</a></li>
    <li><a href="https://github.com/g-truc/glm">glm - OpenGL Mathematics Library</a></li>
    <li><a href="https://github.com/ocornut/imgui">ImGui - UI Library</a></li>
    <li><a href="https://github.com/CedricGuillemet/ImGuizmo">ImGuizmo - Gizmo Library</a></li>
    <li><a href="https://www.mono-project.com/">Mono - .NET Runtime</a></li>
    <li><a href="https://github.com/Chlumsky/msdf-atlas-gen">msdf-atlas-gen - MSDF Font Atlas Generation Utility</a></li>
    <li><a href="https://github.com/gabime/spdlog">spdlog - C++ Logging Library</a></li>
    <li><a href="https://github.com/nothings/stb">stb - Image Loading</a></li>
    <li><a href="https://github.com/jbeder/yaml-cpp">yaml-cpp - YAML Parser & Emitter</a></li>
</ul>

## Compilation

After cloning the latest version of Cataclysm, run Scripts/Win-GenProjects.bat to generate project files for Visual Studio 2022. Currently you can test Cataclysm through your IDE, however complete instructions for compiling binaries are coming soon. Thank you for your patience and for helping to improve Cataclysm!

## Changelog

v1.0.1 

## Documentation

Since C# will be the primary scripting language for Cataclysm, documentation is only be provided for the Cataclysm Core Mono Library (CCML). Some XML documentation may be found throughout the C++ source code for help with C++ scripting, however it is highly recommended that you use C# for all Cataclysm projects. C++ scripting is only supported because the engine is written in C++ and the NativeScript component may be deprecated in a future release.

CCML documentation is be available soon at <a href="https://jackpsmith.com/cataclysm/docs">jackpsmith.com/cataclysm/docs</a>.

## License

Cataclysm is open-source under the MIT License. See LICENSE.txt for details.

## Contributors

Cataclysm is based on the <a href="https://hazelengine.com/">Hazel Engine</a> by <a href="https://github.com/StudioCherno">Studio Cherno</a>. While this repository is not a direct fork
of Hazel, it shares much of the same code. <a href="https://github.com/thecherno">Yan "The Cherno" Chernikov</a>, as well as all members of 
Studio Cherno and all contributors to the Hazel repository are to be considered contributors to this 
project.

Cataclysm is open source, however this main branch in personally maintained by <a href="https://github.com/jackpsmith-git">Jack P Smith</a>.

Author: <a href="https://github.com/jackpsmith-git">Jack P Smith</a> 

Contributors: <a href="https://github.com/jackpsmith-git">Jack P Smith</a>