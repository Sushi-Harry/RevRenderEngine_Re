#version 460

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragWorldPos;
out vec3 FragNormal;
out vec2 FragTexCoords;

layout(std140, binding=0) uniform CameraData{
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ViewProjection;
    vec3 u_ViewPosition;
};

uniform mat4 u_ModelMatrix;

void main(){
    vec4 worldPosition = u_ModelMatrix * vec4(aPosition, 1.0);
    FragWorldPos = worldPosition.xyz;
    FragTexCoords = aTexCoords;
    FragNormal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;

    gl_Position = u_ViewProjection * worldPosition;
}
