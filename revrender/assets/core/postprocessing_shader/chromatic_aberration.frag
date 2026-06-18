#version 460

out vec4 FragColor;

in vec2 u_TexCoords;

layout (binding = 0) uniform sampler2D u_SceneTexture;
uniform float u_Intensity;

void main(){
    vec2 centerVector = u_TexCoords - vec2(0.5);
    float dist = length(centerVector);

    vec2 distortion = centerVector * (dist * dist) * u_Intensity;
    float r = texture(u_SceneTexture, u_TexCoords + distortion).r;
    float g = texture(u_SceneTexture, u_TexCoords).g;
    float b = texture(u_SceneTexture, u_TexCoords - distortion).b;

    float a = texture(u_SceneTexture, u_TexCoords).a;

    FragColor = vec4(r, g, b, a);

}
