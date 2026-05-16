#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
};

uniform vec3 viewPos;

uniform Material material;

uniform DirectionalLight dirLight;

#define NUM_POINT_LIGHTS 4
uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform Spotlight spotlight;


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 normDir = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);
    result += CalcDirLight(dirLight, normDir, viewDir);
    for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
        result += CalcPointLight(pointLights[i], normDir, FragPos, viewDir);
    }
    result += CalcSpotlight(spotlight, normDir, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

    // Ambiet Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse Lighting
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular Lighting
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = ambient + diffuse + specular;

    return result;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Attenuation
    float lightDistance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDistance + 
                 light.quadratic * (lightDistance * lightDistance));    

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

    // Ambiet Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse Lighting
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular Lighting
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = attenuation * (ambient + diffuse + specular);

    return result;
}

vec3 CalcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

    // Ambiet Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse Lighting
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular Lighting
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = intensity * (ambient + diffuse + specular);

    return result;
}