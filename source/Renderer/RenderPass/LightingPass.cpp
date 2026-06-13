#include "LightingPass.h"

#include <glm/gtc/type_ptr.hpp>

LightingPass::LightingPass(const std::string& name, ShaderManager& shaderManager)
	:RenderPass(name),
	IBLBaker(shaderManager)
{
	PBRShader = shaderManager.Get("PBRShader");
}

void LightingPass::Init(RenderContext& context)
{
	PBRShader->use();
	PBRShader->setInt("irradianceMap", 10);
	PBRShader->setInt("prefilterMap", 11);
	PBRShader->setInt("brdfLUT", 12);

	for (int i = 0; i < 4; ++i)
	{
		std::string dirLightName = "dirLightDepthMaps[" + std::to_string(i) + "]";
		PBRShader->setInt(dirLightName, 13 + i);
	}

	IBLBaker.Init();
	context.Scene->Enviroment = std::move(IBLBaker.Bake("content/images/brown_photostudio_02_2k.hdr"));
}

void LightingPass::Resize(unsigned int width, unsigned int height)
{
}

void LightingPass::Execute(RenderContext& context)
{
	context.SceneFramebuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	context.Scene->Enviroment->Bind();
	PBRShader->use();

	UploadCamera(context);
	UploadLight(context);

	DrawPieces(context);

	context.SceneFramebuffer->UnBind();
}


void LightingPass::UploadCamera(RenderContext& context)
{
	glm::mat4 projection = glm::perspective(context.Scene->MainCamera->GetFoV(), 
		static_cast<float>(context.ScreenWidth) / static_cast<float>(context.ScreenHeight),
		0.1f, 100.0f);

	PBRShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(projection));
}

void LightingPass::UploadLight(RenderContext& context)
{
	auto scene = context.Scene;

	std::string name;
	int i = 0;
	for (; i < scene->DirectionalLights.size(); ++i)
	{
		name = "lights[" + std::to_string(i) + "]";
		scene->DirectionalLights[i].Upload(PBRShader.get(), name);
	}

	for (; i < scene->PointLights.size(); ++i)
	{
		name = "lights[" + std::to_string(i) + "]";
		scene->PointLights[i].Upload(PBRShader.get(), name);
	}

	for (; i < scene->SpotLights.size(); ++i)
	{
		scene->SpotLights[i].SetPosition(scene->MainCamera->Pos);
		scene->SpotLights[i].SetDirection(scene->MainCamera->GetFront());
		name = "lights[" + std::to_string(i) + "]";
		scene->SpotLights[i].Upload(PBRShader.get(), name);
	}
	PBRShader->setInt("num_lights", i);
}

void LightingPass::DrawPieces(RenderContext& context)
{
	auto scene = context.Scene;

	glm::mat4 view = scene->MainCamera->GetViewMatrix();

	PBRShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(view));
	PBRShader->setVec3("camPos", scene->MainCamera->Pos);

	for (int i = 0; i < scene->Pieces.size(); ++i)
	{
		glm::mat4 model_normal = glm::transpose(glm::inverse(scene->Pieces[i]->Transform));

		PBRShader->setMat4("model", 1, GL_FALSE, glm::value_ptr(scene->Pieces[i]->Transform));
		PBRShader->setMat4("model_normal", 1, GL_FALSE, glm::value_ptr(model_normal));

		scene->Pieces[i]->ModelPtr->Draw(PBRShader.get());
	}
}
