#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 u_TexCoords;
out vec3 u_Normal;
out vec3 u_FragPos;

layout (location = 0) uniform mat4 u_ViewProjection;
layout (location = 1) uniform mat4 u_ModelMatrix;


void main(){
    u_TexCoords = aTexCoords;
    u_Normal = aNormal;
    u_FragPos = vec3(u_ModelMatrix * vec4(aPosition, 1.0));

    gl_Position = u_ViewProjection * u_ModelMatrix * vec4(aPosition, 1.0);
}
