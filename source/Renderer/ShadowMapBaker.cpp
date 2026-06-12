#include "ShadowMapBaker.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


ShadowMapBaker::ShadowMapBaker(ShaderManager& shaderManager)
{
	SimpleDepthShader = shaderManager.Get("SimpleDepthShader");
}

void ShadowMapBaker::Init(Scene* scene)
{
	for (int i = 0; i < scene->DirectionalLights.size(); ++i)
	{
		DirectionalLight& dirLight = scene->DirectionalLights[i];

		glGenFramebuffers(1, &dirLight.Shadow.DepthMapFBO);

		glGenTextures(1, &dirLight.Shadow.DepthMap);
		glBindTexture(GL_TEXTURE_2D, dirLight.Shadow.DepthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			dirLight.Shadow.Width, dirLight.Shadow.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, dirLight.Shadow.DepthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dirLight.Shadow.DepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapBaker::Bake(Scene* scene)
{
	SimpleDepthShader->use();
	// Save current viewport size
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int scrWidth = viewport[2];
	int scrHeight = viewport[3];

	for (int i = 0; i < scene->DirectionalLights.size(); ++i)
	{
		DirectionalLight& dirLight = scene->DirectionalLights[i];

		glm::mat4 lightProjection, lightView;

		dirLight.Shadow.LightSpaceMatrix = ComputeLightSpaceMatrix(-scene->DirectionalLights[i].GetDirection());

		SimpleDepthShader->setMat4("lightSpaceMatrix", 1, GL_FALSE,
			glm::value_ptr(dirLight.Shadow.LightSpaceMatrix));

		// render scene from light's point of view
		glViewport(0, 0, dirLight.Shadow.Width, dirLight.Shadow.Height);
		glBindFramebuffer(GL_FRAMEBUFFER, dirLight.Shadow.DepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		DrawPieces(scene);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// reset viewport
	glViewport(0, 0, scrWidth, scrHeight);
}

glm::mat4 ShadowMapBaker::ComputeLightSpaceMatrix(glm::vec3 lightPos)
{
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, near_plane, far_plane);

	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

void ShadowMapBaker::DrawPieces(Scene* scene)
{
	SimpleDepthShader->use();

	for (int i = 0; i < scene->Pieces.size(); ++i)
	{
		SimpleDepthShader->setMat4("model", 1, GL_FALSE, glm::value_ptr(scene->Pieces[i]->Transform));
		scene->Pieces[i]->ModelPtr->Draw(SimpleDepthShader.get());
	}
}

