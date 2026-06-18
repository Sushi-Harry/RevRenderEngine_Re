#version 460 core

out vec4 FragColor;

in vec2 u_TexCoords;

layout (binding = 0) uniform sampler2D u_SceneTexture;

uniform float u_BlockSize;
uniform vec2 u_Resolution;

void main(){
    vec2 blockCount = u_Resolution / u_BlockSize;

    vec2 uv = floor(u_TexCoords * blockCount) / blockCount;

    vec3 color = texture(u_SceneTexture, uv).rgb;
    FragColor = vec4(color, 1.0);
}
