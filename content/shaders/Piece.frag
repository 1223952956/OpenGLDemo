#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

out vec4 FragColor;

#define PI 3.14159265358979323846
#define MAX_LIGHTS 16
#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2

uniform sampler2D texture_base_color;
uniform vec4 base_color_factor;

uniform sampler2D texture_normal;
uniform float normal_scale;

uniform sampler2D texture_metallic_roughness;
uniform float metallic_factor;
uniform float roughness_factor;

uniform sampler2D texture_emissive;
uniform vec3 emissive_factor;

uniform sampler2D texture_occlusion;
uniform float occlusion_strength;

uniform bool is_opaque;
uniform float alpha_cutoff;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;  


// TODO 
// sperate light to Point/Directional/Spot 
struct Light {
    vec3 position;   
    vec3 color;      
    int type;     // Use int may have memory alignment issue  
    float range;
    float intensity;

    vec3 direction;

    //spot
    float innerCos;
    float outerCos;

    // point
    float constant;
    float linear;
    float quadratic;
};

uniform int num_lights;
uniform Light lights[MAX_LIGHTS];

uniform vec3 camPos;

// PBR Tool Function
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdH = max(dot(N, H), 0.0);
    float d = NdH * NdH * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float GeometrySmith(float NdV, float NdL, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float g1 = NdV / (NdV * (1.0 - k) + k);
    float g2 = NdL / (NdL * (1.0 - k) + k);
    return g1 * g2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float CalcShadow(vec4 fragPosLightSpace)
{
    return 0.f;
}

vec3 CalcLightRadiance(Light light, vec3 fragPos, vec3 N, vec3 V,
                       vec3 albedo, float metallic, float roughness) {
    // 1. calc L and attenuation
    vec3  L;
    float attenuation = 1.0;

    // "If" may be slow on GPU
    if (light.type == LIGHT_POINT) {
        // point
        vec3  delta = light.position - fragPos;
        float dist  = length(delta);
        L = normalize(delta);
        attenuation = 1.0 / (dist * dist);
        float rangeFactor = clamp(1.0 - pow(dist / light.range, 4.0), 0.0, 1.0);
        attenuation *= rangeFactor * rangeFactor;

    } else if (light.type == LIGHT_DIRECTIONAL) {
        // directional
        L = normalize(-light.direction);

    } else if (light.type == LIGHT_SPOT) {
        // spot
        vec3  delta    = light.position - fragPos;
        float dist     = length(delta);
        L = normalize(delta);
        float cosTheta = dot(-L, normalize(light.direction));
        float epsilon  = light.innerCos - light.outerCos;
        float spotFade = clamp((cosTheta - light.outerCos) / epsilon, 0.0, 1.0);
        float diff = max(dot(normalize(Normal), normalize(L)), 0.0);
        attenuation = spotFade / (1.0 + dist * dist);
        float rangeFactor = clamp(1.0 - pow(dist / light.range, 4.0), 0.0, 1.0);
        attenuation *= rangeFactor * rangeFactor;
    }

    // 2. shared
    vec3  H   = normalize(V + L);
    float NdL = max(dot(N, L), 0.0);
    float NdV = max(dot(N, V), 0.0);
    float HdV = max(dot(H, V), 0.0);

    if (NdL < 1e-4) return vec3(0.0); // Ignore back

    // 3. PBR BRDF
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(NdV, NdL, roughness);
    vec3  F = fresnelSchlick(HdV, F0);

    vec3  specular  = (D * G * F) / max(4.0 * NdV * NdL, 1e-4);
    vec3  kD        = (1.0 - F) * (1.0 - metallic);
    vec3  diffuse   = kD * albedo / PI;

    // 4. shadow
    float shadow = CalcShadow(FragPosLightSpace);

    // 5. merge
    vec3 radiance = light.intensity * light.color * attenuation;
    return (1.0 - shadow) * (diffuse + specular) * radiance * NdL;
}



// ACES Film Tone Mapping
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


void main()
{
    vec3 norm = normalize(Normal);
    vec3 view = normalize(camPos - WorldPos);
    vec3 R = reflect(-view, norm);   

    vec3 albedo = texture(texture_base_color, TexCoords).rgb;
    float roughness  = texture(texture_metallic_roughness, TexCoords).g * roughness_factor;
    float metallic  = texture(texture_metallic_roughness, TexCoords).b * metallic_factor;
    float ao = texture(texture_occlusion, TexCoords).r * occlusion_strength;
    vec3 emissive = texture(texture_emissive, TexCoords).rgb * emissive_factor;

    // Add per light
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < num_lights; i++) {
        Lo += CalcLightRadiance(lights[i], WorldPos, norm, view, albedo, metallic, roughness);
    }

    // IBL
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = FresnelSchlickRoughness(max(dot(norm, view), 0.0), F0, roughness);
    vec3 kD = (1.0 - F) * (1.0 - metallic);
    
    vec3 irradiance = texture(irradianceMap, norm).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;   
    vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(norm, view), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = emissive + Lo + kD * ambient;

    // ACE
    color = ACESFilm(color);  
    // No need for double gamma correction for that gltf texure are RGBA
    // color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}