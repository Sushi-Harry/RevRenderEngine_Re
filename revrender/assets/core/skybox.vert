#version 460 core

layout (location = 0) in vec3 aPosition;

out vec3 u_TexCoords;

layout (location = 0) uniform mat4 u_ViewProjection;

void main()
{
    u_TexCoords = aPosition;
    vec4 pos = u_ViewProjection * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}
