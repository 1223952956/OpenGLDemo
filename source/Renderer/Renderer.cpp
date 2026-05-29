#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "RenderPrimitives.h"

Renderer::Renderer()
	: PBRShader("content/shaders/Piece.vert", "content/shaders/Piece.frag"),
	ShadowShader("content/shaders/SimpleDepth", "content/shaders/SimpleDepth"),
	SkyboxShader("content/shaders/SkyBox.vert", "content/shaders/SkyBox.frag")
{
	PBRShader.use();
	PBRShader.setInt("irradianceMap", 10);
	PBRShader.setInt("prefilterMap", 11);
	PBRShader.setInt("brdfLUT", 12);

	SkyboxShader.setInt("environmentMap", 0);
}

void Renderer::Render(Scene& scene, float screenWidth, float screenHeight)
{
	glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UploadCamera(scene, screenWidth, screenHeight);
	UploadLights(scene);

	scene.Enviroment->Bind();

	DrawPieces(scene);
	DrawSkyBox(scene);
}

void Renderer::UploadCamera(Scene& scene, float screenWidth, float screenHeight)
{
	glm::mat4 projection = glm::perspective(scene.MainCamera->GetFoV(), screenWidth / screenHeight, 0.1f, 100.0f);
	PBRShader.use();
	PBRShader.setMat4("projection", 1, GL_FALSE, glm::value_ptr(projection));
	SkyboxShader.use();
	SkyboxShader.setMat4("projection", 1, GL_FALSE, glm::value_ptr(projection));
}

void Renderer::UploadLights(Scene& scene)
{
	PBRShader.use();
	
	std::string name;
	int i = 0;
	for (; i < scene.DirectionalLights.size(); ++i)
	{
		name = "lights[" + std::to_string(i) + "]";
		scene.DirectionalLights[i].Upload(PBRShader, name);
	}

	for (; i < scene.PointLights.size(); ++i)
	{
		name = "lights[" + std::to_string(i) + "]";
		scene.PointLights[i].Upload(PBRShader, name);
	}

	for (; i < scene.SpotLights.size(); ++i)
	{
		scene.SpotLights[i].SetPosition(scene.MainCamera->Pos);
		scene.SpotLights[i].SetDirection(scene.MainCamera->GetFront());
		name = "lights[" + std::to_string(i) + "]";
		scene.SpotLights[i].Upload(PBRShader, name);
	}
	PBRShader.setInt("num_lights", i);
}

void Renderer::DrawPieces(Scene& scene)
{
	PBRShader.use();
	
	glm::mat4 view = scene.MainCamera->GetViewMatrix();
	
	PBRShader.setMat4("view", 1, GL_FALSE, glm::value_ptr(view));
	PBRShader.setVec3("camPos", scene.MainCamera->Pos);

	for (int i = 0; i < scene.Pieces.size(); ++i)
	{
		glm::mat4 model_normal = glm::transpose(glm::inverse(scene.Pieces[i]->Transform));
		
		PBRShader.setMat4("model", 1, GL_FALSE, glm::value_ptr(scene.Pieces[i]->Transform));
		PBRShader.setMat4("model_normal", 1, GL_FALSE, glm::value_ptr(model_normal));
		
		scene.Pieces[i]->ModelPtr->Draw(PBRShader);
	}
}

void Renderer::DrawSkyBox(Scene& scene)
{
	glDepthFunc(GL_LEQUAL);

	SkyboxShader.use();

	glm::mat4 view = scene.MainCamera->GetViewMatrix();

	SkyboxShader.setMat4("view", 1, GL_FALSE, glm::value_ptr(view));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.Enviroment->EnvCubeMap.ID);
	RenderPrimitives::RenderCube();

	glDepthFunc(GL_LESS);
}
