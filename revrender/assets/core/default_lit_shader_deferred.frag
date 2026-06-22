#version 460 core

out vec4 FragColor;
in vec2 u_TexCoords;

layout (binding = 0) uniform sampler2D gAlbedoSpec;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gPosition;

// 48+64 Bytes after 8 bytes of padding
struct DirectionalLight{
    // 16 Bytes
    vec3 _direction;
    bool _enabled;

    // 16 Bytes
    vec3 _color;
    float _ambient;

    // 64 Bytes
    mat4 _light_space_matrix;

    // 8 Bytes
    float _specular;
    float _diffuse;
    // Another 8 bytes padded
};

// 64 Bytes (after 4 bytes of padding)
struct PointLight{
    // 16 Bytes
    vec3 _position;
    bool _enabled;

    // 16 Bytes
    vec3 _color;
    float _ambient;

    // 16 Bytes
    float _constant;
    float _linear;
    float _quadratic;
    float _specular;

    // 4 Bytes
    float _diffuse;

    // 12 bytes of padding
};

// 144 Bytes (after 8 bytes of padding)
struct SpotLight{
    // 16 Bytes
    vec3 _position;
    bool _enabled;

    // 16 Bytes
    vec3 _direction;
    float _constant;

    // 16 Bytes
    vec3 _color;
    float _linear;

    // 64 Bytes
    mat4 _light_space_matrix;

    // 16 Bytes
    float _quadratic;
    float _ambient;
    float _specular;
    float _diffuse;

    // 8 Bytes.
    float _inner_cutoff;
    float _outer_cutoff;

    // Another padded 8 Bytes.
};

#define MAX_SPOT_LIGHTS 4
#define MAX_POINT_LIGHTS 4

layout(std140, binding=0) uniform CameraData {
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ViewProjection;
    vec3 u_ViewPosition;
};

layout(std140, binding=1) uniform LightData {
    DirectionalLight u_DirectionalLight;
    PointLight u_PointLights[MAX_POINT_LIGHTS];
    SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
};

uniform samplerCube u_Skybox;

uniform sampler2DArray u_ShadowMaps_SpotLight;
uniform sampler2D u_ShadowMap;

float calc_shadows_directional(vec3 lightDir, vec4 fragPosLightSpace, vec3 normal);
float calc_shadows_spot(vec3 lightDir, vec4 fragPosLightSpace, vec3 normal, int layerIndex);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texDiffuse, vec3 texSpecular, float shininess, int layerIndex);
vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texDiffuse, vec3 texSpecular, float shininess);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texDiffuse, vec3 texSpecular, float shininess);

vec3 calc_environment_reflection(samplerCube env_cmap, vec3 normal, vec3 fragpos, vec3 camerapos){
    vec3 I = normalize(fragpos - camerapos);
    vec3 R = reflect(I, normalize(normal));
    vec3 reflectColor = texture(env_cmap, R).rgb;
    return reflectColor;
}

void main() {

    vec3 fragPos = texture(gPosition, u_TexCoords).rgb;
    vec4 normalSample = texture(gNormal, u_TexCoords);
    vec3 norm = normalSample.rgb;

    // if normal is empty, we are looking at the sky background
    if (length(norm) == 0.0) {
        discard;
    }
    vec4 albedoSample = texture(gAlbedoSpec, u_TexCoords);
    vec3 diffColor = albedoSample.rgb;         // Extracted diffuse color!
    vec3 specColor = vec3(albedoSample.a);     // Extracted specular intensity!

    float shininess = 32.0;

    vec3 viewDir = normalize(u_ViewPosition - fragPos);
    vec3 result = vec3(0.0);

    float material_reflectivity = pow(1.0 - max(dot(norm, viewDir), 0.0), 5.0);

    for(int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        if(u_SpotLights[i]._enabled) {
            result += calc_spot_light(u_SpotLights[i], norm, fragPos, viewDir, diffColor, specColor, shininess, i);
        }
    }

    for(int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if(u_PointLights[i]._enabled) {
            result += calc_point_light(u_PointLights[i], norm, fragPos, viewDir, diffColor, specColor, shininess);
        }
    }

    result += calc_directional_light(u_DirectionalLight, norm, fragPos, viewDir, diffColor, specColor, shininess);
    vec3 reflectionColor = calc_environment_reflection(u_Skybox, norm, fragPos, u_ViewPosition);
    vec3 finalColor = mix(result, reflectionColor, material_reflectivity);

    FragColor = vec4(finalColor, 1.0);
    // FragColor = vec4(texture(gAlbedoSpec, u_TexCoords).rgb, 1.0);
}


vec3 calc_point_light(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texDiffuse, vec3 texSpecular, float shininess) {
    if(!light._enabled) return vec3(0.0);

    vec3 lightDir = normalize(light._position - fragPos);
    float dist = length(light._position - fragPos);
    float attenuation = 1.0 / (light._constant + (light._linear * dist) + (light._quadratic * dist * dist));
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light._color * light._ambient * texDiffuse;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light._color * light._diffuse * diff * texDiffuse;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light._color * spec * light._specular * texSpecular;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texDiffuse, vec3 texSpecular, float shininess, int layerIndex) {
    if(!light._enabled) return vec3(0.0);

    vec3 lightDir = normalize(light._position - fragPos);
    float theta = dot(lightDir, normalize(-light._direction));

    float epsilon = light._inner_cutoff - light._outer_cutoff;
    float intensity = clamp((theta - light._outer_cutoff) / epsilon, 0.0, 1.0);

    float dist = length(light._position - fragPos);
    float attenuation = 1.0 / (light._constant + (light._linear * dist) + (light._quadratic * dist * dist));
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light._color * light._ambient * texDiffuse;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light._color * light._diffuse * diff * texDiffuse;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light._color * spec * light._specular * texSpecular;

    vec4 fragPosLightSpace = light._light_space_matrix * vec4(fragPos, 1.0);
    float shadow = calc_shadows_spot(lightDir, fragPosLightSpace, normal, layerIndex);

    return (ambient + ((1.0 - shadow) * (diffuse + specular) * intensity)) * attenuation;
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texDiffuse, vec3 texSpecular, float shininess) {
    if(!light._enabled) return vec3(0.0);

    vec3 lightDir = normalize(-light._direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light._color * light._ambient * texDiffuse;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light._color * light._diffuse * diff * texDiffuse;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light._color * light._specular * spec * texSpecular;

    float shadow = calc_shadows_directional(normalize(-light._direction), (light._light_space_matrix * vec4(fragPos, 1.0)), normal);

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

float calc_shadows_directional(vec3 lightDir, vec4 fragPosLightSpace, vec3 normal) {
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

float calc_shadows_spot(vec3 lightDir, vec4 fragPosLightSpace, vec3 normal, int layerIndex) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    float shadow = 0.0;

    vec2 texelSize = 1.0 / vec2(textureSize(u_ShadowMaps_SpotLight, 0).xy);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            vec3 uvLayer = vec3(projCoords.xy + vec2(x, y) * texelSize, float(layerIndex));
            float pcfDepth = texture(u_ShadowMaps_SpotLight, uvLayer).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}
