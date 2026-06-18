#version 460

// YOU CAN PROBABLY DEDUCE THIS EASILY BUT THIS SHADER WASN'T WRITTEN BY ME AT ALL

out vec4 FragColor;

in vec2 u_TexCoords;

uniform float u_Time;
uniform float u_Intensity;
layout (binding = 0) uniform sampler2D u_SceneTexture;

float hash2d(uvec2 p){
    p = p * uvec2(1597334677U, 3817116569U);
    uint n = (p.x ^ p.y) *1597334677U;
    return float(n) /4294967295.0;
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main(){
    vec4 baseColor = texture(u_SceneTexture, u_TexCoords);

    uvec2 pixelCoords = uvec2(u_TexCoords.xy);
    uint timeSeed = uint(u_Time * 60.0);
    pixelCoords.x *=timeSeed;
    pixelCoords.y *=timeSeed;

    float noise = hash2d(pixelCoords);
    vec3 grain = vec3(noise - 0.5) * u_Intensity;
    vec3 finalColor = baseColor.rgb + grain;

    FragColor = vec4(finalColor, baseColor.a);
}
