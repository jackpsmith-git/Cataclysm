// Line Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform vec4 u_Color;

out mat4 v_ViewProjection;
out vec4 v_Color;
out flat int v_EntityID;

void main()
{
    v_Color = a_Color;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

uniform vec4 u_Color;

layout(location = 0) in vec4 v_Color;
layout(location = 1) in flat int v_EntityID;

void main()
{
    o_Color = v_Color;
    o_EntityID = v_EntityID;
}