#version 460

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gPosition;
layout (location = 3) out int gEntityID;

in vec3 FragWorldPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

struct Material{
    sampler2D _texture_diffuse;
    sampler2D _texture_specular;
    float _shininess;
};

uniform Material u_Material;
uniform int u_EntityID;

void main(){
    vec4 diffuseTexColor = texture(u_Material._texture_diffuse, FragTexCoords);
    if(diffuseTexColor.a < 0.1) discard;

    gAlbedoSpec.rgb = diffuseTexColor.rgb;
    gAlbedoSpec.a = texture(u_Material._texture_specular, FragTexCoords).r;
    gNormal.rgb = normalize(FragNormal);
    gNormal.a = u_Material._shininess / 256.0;
    gPosition = FragWorldPos;
    gEntityID = u_EntityID;
}
