#include "Scene.h"
#include "Scene/Components/ModelComponent.h"
#include "Math/Transform.h"

void Scene::Initialize()
{
	MainCamera = std::make_unique<Camera>();
	MainCamera->Pos = glm::vec3(0.0f, 0.4f, 1.8f);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.5f,  3.0f,  2.0f),
		glm::vec3(-3.0f,  1.0f,  2.0f),
		glm::vec3(-2.0f,  2.0f, -3.0f),
		glm::vec3(0.0f,  5.0f, 0.0f)
	};

	// Model Initialize

	auto ChessBoardModelPtr = std::make_shared<Model>("content/models/ChessBoard.glb");
	auto BlackQueenModelPtr = std::make_shared<Model>("content/models/BlackQueen.glb");
	auto WhiteRookModelPtr = std::make_shared<Model>("content/models/WhiteKnight.glb");

	auto& ChessBoard = CreatePiece();
	auto& BlackQueen = CreatePiece();
	auto& WhiteRook = CreatePiece();
	
	ChessBoard.SetRootComponent(&ChessBoard.AddComponent<ModelComponent>(ChessBoardModelPtr));
	BlackQueen.SetRootComponent(&BlackQueen.AddComponent<ModelComponent>(BlackQueenModelPtr));
	WhiteRook.SetRootComponent(&WhiteRook.AddComponent<ModelComponent>(WhiteRookModelPtr));

	Math::Transform model;

	for (int i = 0; i < Pieces.size(); ++i)
	{
		Pieces[i]->SetLocalTransform(model);
		Pieces[i]->Initialize();
	}

	// Light Initialize
	DirectionalLights.emplace_back(DirectionalLight(glm::vec3(-2.0f, -2.0f, -1.0f), glm::vec3(1.0f, 0.95f, 0.90f), 4.0f));
	for (int i = 0; i < 4; ++i)
	{
		PointLights.emplace_back(PointLight(pointLightPositions[i], glm::vec3(1.0f, 0.97f, 0.92f), 10.f, 0.0f));
	}
}

void Scene::Update(float deltaTime)
{
	for (auto& piece : Pieces)
	{
		piece->Update(deltaTime);
	}
}

void Scene::Uninitialize()
{
	for (auto& piece : Pieces)
	{
		piece->Uninitialize();
	}
}

Piece& Scene::CreatePiece()
{
    auto piece = std::make_unique<Piece>();

    Piece& ref = *piece;

    Pieces.push_back(std::move(piece));

    return ref;
}
