#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 u_TexCoords;
out vec3 u_Normal;
out vec3 u_FragPos;
out vec4 u_FragPosLightSpace;

layout (location = 0) uniform mat4 u_ViewProjection;
layout (location = 1) uniform mat4 u_ModelMatrix;
layout (location = 2) uniform mat4 u_LightSpaceMatrix;

void main(){
    u_TexCoords = aTexCoords;
    u_Normal = mat3(u_ModelMatrix) * aNormal;
    u_FragPos = vec3(u_ModelMatrix * vec4(aPosition, 1.0));
    u_FragPosLightSpace = u_LightSpaceMatrix * vec4(u_FragPos, 1.0);

    gl_Position = u_ViewProjection * u_ModelMatrix * vec4(aPosition, 1.0);
}
