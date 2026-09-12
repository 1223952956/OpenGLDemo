#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define MAX_DIR_LIGHTS 4

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out vec4 FragPosLightSpace[MAX_DIR_LIGHTS];


uniform mat4 model;
uniform mat4 model_normal;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 dirLightSpaceMatrices[MAX_DIR_LIGHTS];

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model_normal) * aNormal;

    for (int i = 0; i < MAX_DIR_LIGHTS; ++i) 
    {
        FragPosLightSpace[i] = dirLightSpaceMatrices[i] * vec4(WorldPos, 1.0);
    }
}