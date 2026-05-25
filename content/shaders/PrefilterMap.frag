#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}