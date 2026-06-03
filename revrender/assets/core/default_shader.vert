#version 450 core

layout (location = 0) in vec3 aPos;

layout (location = 0) uniform mat4 u_ViewProjection;
layout (location = 1) uniform mat4 u_ModelMatrix;

void main(){
    gl_Position = u_ViewProjection * u_ModelMatrix * vec4(aPos, 1.0);
}
