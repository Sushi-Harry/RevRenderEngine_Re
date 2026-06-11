#version 460 core

layout (location = 0) in vec3 aPosition;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_ModelMatrix;

void main(){
    gl_Position = u_LightSpaceMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
}
