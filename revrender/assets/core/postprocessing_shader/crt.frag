#version 460

out vec4 FragColor;

in vec2 u_TexCoords;

layout (binding=0) uniform sampler2D u_SceneTexture;

uniform float u_LineCount;
uniform float u_Intensity;
void main(){

    vec4 baseColor = texture(u_SceneTexture, u_TexCoords);
    float scanline = sin(u_TexCoords.y * u_LineCount * 3.14159265);

    scanline = mix(1.0, ((scanline * 0.5) + 0.5), u_Intensity);

    vec3 finalColor = baseColor.rgb * scanline * 1.1;
    FragColor = vec4(finalColor, baseColor.a);
}
