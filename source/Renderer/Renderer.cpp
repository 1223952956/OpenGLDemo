#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "RenderPrimitives.h"

Renderer::Renderer()
	: PBRShader("content/shaders/Piece.vert", "content/shaders/Piece.frag"),
	SkyboxShader("content/shaders/SkyBox.vert", "content/shaders/SkyBox.frag"),
	DebugQuadShader("content/shaders/DebugQuad.vert", "content/shaders/DebugQuad.frag")
{
	PBRShader.use();
	PBRShader.setInt("irradianceMap", 10);
	PBRShader.setInt("prefilterMap", 11);
	PBRShader.setInt("brdfLUT", 12);

	for (int i = 0; i < 4; ++i)
	{
		std::string dirLightName = "dirLightDepthMaps[" + std::to_string(i) + "]";
		PBRShader.setInt(dirLightName, 13 + i);
	}
	SkyboxShader.use();
	SkyboxShader.setInt("environmentMap", 0);

	DebugQuadShader.use();
	DebugQuadShader.setInt("depthMap", 0);
}

void Renderer::Init(Scene& scene)
{
	ShadowBaker.Init(scene);
}

void Renderer::Render(Scene& scene, float screenWidth, float screenHeight)
{
	glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UploadCamera(scene, screenWidth, screenHeight);
	UploadLights(scene);

	ShadowBaker.Bake(scene);

	scene.Enviroment->Bind();

	DrawShadow(scene);
	DrawPieces(scene);
	DrawSkyBox(scene);

	// DrawDebugQuad(scene);
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

void Renderer::DrawShadow(Scene& scene)
{
	PBRShader.use();

	for (int i = 0; i < scene.DirectionalLights.size(); ++i)
	{
		DirectionalLight& dirLight = scene.DirectionalLights[i];
		std::string dirLightName = "dirLightSpaceMatrices[" + std::to_string(i) + "]";
		PBRShader.setMat4(dirLightName, 1, GL_FALSE, glm::value_ptr(dirLight.Shadow.LightSpaceMatrix));
		glActiveTexture(GL_TEXTURE13 + i);
		glBindTexture(GL_TEXTURE_2D, dirLight.Shadow.DepthMap);
	}
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

void Renderer::DrawDebugQuad(Scene& scene)
{
	DebugQuadShader.use();
	DebugQuadShader.setFloat("near_plane", 1.0f);
	DebugQuadShader.setFloat("far_plane", 7.5f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene.DirectionalLights[0].Shadow.DepthMap);
	RenderPrimitives::RenderQuad();
}
