#pragma once
#include <memory>

#include "Piece.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Renderer/IBLMaterial.h"
#include "Camera.h"

class Scene
{
public:
	std::vector<std::unique_ptr<Piece>> Pieces;

	std::vector<DirectionalLight> DirectionalLights;
	std::vector<PointLight> PointLights;
	std::vector<SpotLight> SpotLights;

	std::string EnvironmentPath;
	std::unique_ptr<IBLMaterial> Environment;

	std::unique_ptr<Camera> MainCamera;

	void Initialize();
	void Update(float deltaTime);
	void Uninitialize();
	Piece& CreatePiece();
	Piece& CreatePiece(UUID id);

private:
	void CreateDefaultScene();
};

