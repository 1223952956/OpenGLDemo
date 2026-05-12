#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


void main()
{
    // Ambiet Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse Lighting
    vec3 normDir = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(normDir, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    // Specular Lighting
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normDir);

    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    // Add all
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}