#version 460 core

out vec4 FragColor;

in vec2 u_TexCoords;
in vec3 u_Normal;
in vec3 u_FragPos;
in vec4 u_FragPosLightSpace;

struct Material{
    sampler2D _texture_diffuse;
    sampler2D _texture_specular;
    float _shininess;
};

struct DirectionalLight{
    bool _enabled;

    vec3 _direction;
    vec3 _color;

    float _ambient;
    float _specular;
    float _diffuse;
};

struct PointLight{
    bool _enabled;
    vec3 _position;

    vec3 _color;

    // Attenuation variables
    float _constant;
    float _linear;
    float _quadratic;

    // Attribute strengths
    float _ambient;
    float _specular;
    float _diffuse;
};

#define MAX_POINT_LIGHTS 4

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight u_DirectionalLight;
uniform sampler2D u_ShadowMap;

float calc_shadows_directional(vec3 lightDir, vec4 fragPosLightSpace, vec3 normal);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat);
vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat);

void main(){
    vec4 diffuseTexColor = texture(u_Material._texture_diffuse, u_TexCoords);
    vec4 specularTexColor = texture(u_Material._texture_specular, u_TexCoords);

    if(diffuseTexColor.a < 0.1) discard;

    vec3 norm = normalize(u_Normal);
    vec3 viewDir = normalize(u_ViewPos - u_FragPos);

    vec3 result = vec3(0.0);

    for(int i = 0; i < MAX_POINT_LIGHTS; i++){
        if(u_PointLights[i]._enabled){
            result += calc_point_light(u_PointLights[i], norm, u_FragPos, viewDir, u_Material);
        }
    }

    result += calc_directional_light(u_DirectionalLight, norm, u_FragPos, viewDir, u_Material);

    FragColor = vec4(result, diffuseTexColor.a);
    // FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat){

    if(!light._enabled){
        return vec3(0.0);
    }

    vec3 lightDir = normalize(light._position - fragPos);
    float dist = length(light._position - fragPos);
    float attenuation = 1.0 / (light._constant + (light._linear * dist) + (light._quadratic * dist * dist));
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light._color * light._ambient * texture(mat._texture_diffuse, u_TexCoords).rgb;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light._color * light._diffuse * diff * texture(mat._texture_diffuse, u_TexCoords).rgb;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat._shininess);
    vec3 specular = light._color * spec * light._specular * texture(mat._texture_specular, u_TexCoords).rgb;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat){
    if(!light._enabled){
        return vec3(0.0);
    }

    vec3 lightDir = normalize(-light._direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light._color * light._ambient * texture(mat._texture_diffuse, u_TexCoords).rgb;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light._color * light._diffuse * diff * texture(mat._texture_diffuse, u_TexCoords).rgb;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat._shininess);
    vec3 specular = light._color * light._specular * spec * texture(mat._texture_specular, u_TexCoords).rgb;

    float shadow = calc_shadows_directional(normalize(-light._direction), u_FragPosLightSpace, normal);

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

float calc_shadows_directional(vec3 lightDir, vec4 fragPosLightSpace, vec3 normal){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}
