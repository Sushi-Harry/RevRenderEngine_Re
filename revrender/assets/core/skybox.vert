#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (location = 0) uniform mat4 u_ViewProjection;
layout (location = 1) uniform mat4 u_ModelMatrix;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_ViewProjection * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
