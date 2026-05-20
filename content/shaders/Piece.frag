#version 330 core

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    sampler2D texture_base_color;
    sampler2D texture_normal;
    sampler2D texture_metallic_roughness;
    sampler2D texture_emissive;
    sampler2D texture_occlusion;
};

uniform Material material;

uniform vec3 camPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view = normalize(camPos - WorldPos);

    vec4 baseColor = texture(material.texture_base_color, TexCoords);
    vec4 emissionColor = texture(material.texture_emissive, TexCoords);
    FragColor = baseColor + emissionColor;
}