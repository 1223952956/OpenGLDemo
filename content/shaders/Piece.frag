#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D texture_base_color;
    sampler2D texture_normal;
    sampler2D texture_metallic_roughness;
    sampler2D texture_emissive;
    sampler2D texture_occlusion;
};

uniform Material material;

void main()
{
    FragColor = texture(material.texture_base_color, TexCoords);
}