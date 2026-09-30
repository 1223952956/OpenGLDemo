#include "Scene.h"
#include "Scene/Components/ModelComponent.h"
#include "Math/Transform.h"

void Scene::Initialize()
{
	for (int i = 0; i < Pieces.size(); ++i)
	{
		Pieces[i]->Initialize();
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

Piece& Scene::CreatePiece(UUID id)
{
	auto piece = std::make_unique<Piece>(id);

	Piece& ref = *piece;

	Pieces.push_back(std::move(piece));

	return ref;
}

void Scene::CreateDefaultScene()
{
	MainCamera = std::make_unique<Camera>();
	MainCamera->Pos = glm::vec3(0.0f, 0.4f, 1.8f);

	EnvironmentPath = "content/images/fireplace_4k.hdr";

	//glm::vec3 pointLightPositions[] = {
	//	glm::vec3(2.5f,  3.0f,  2.0f),
	//	glm::vec3(-3.0f,  1.0f,  2.0f),
	//	glm::vec3(-2.0f,  2.0f, -3.0f),
	//	glm::vec3(0.0f,  5.0f, 0.0f)
	//};

	// Model Initialize

	auto ChessBoardModelPtr = std::make_shared<Model>("content/models/ChessBoard.glb");
	auto BlackQueenModelPtr = std::make_shared<Model>("content/models/BlackQueen.glb");
	auto BlackRookModelPtr = std::make_shared<Model>("content/models/BlackRook.glb");
	auto BlackKnightModelPtr = std::make_shared<Model>("content/models/BlackKnight.glb");
	auto BlackBishopModelPtr = std::make_shared<Model>("content/models/BlackBishop.glb");
	auto BlackKingModelPtr = std::make_shared<Model>("content/models/BlackKing.glb");
	auto BlackPawnModelPtr = std::make_shared<Model>("content/models/BlackPawn.glb");
	auto WhiteRookModelPtr = std::make_shared<Model>("content/models/WhiteKnight.glb");
	auto WhiteKnightModelPtr = std::make_shared<Model>("content/models/WhiteKnight.glb");
	auto WhiteBishopModelPtr = std::make_shared<Model>("content/models/WhiteBishop.glb");
	auto WhiteQueenModelPtr = std::make_shared<Model>("content/models/WhiteQueen.glb");
	auto WhiteKingModelPtr = std::make_shared<Model>("content/models/WhiteKing.glb");
	auto WhitePawnModelPtr = std::make_shared<Model>("content/models/WhitePawn.glb");

	auto& ChessBoard = CreatePiece();
	auto& BlackQueen = CreatePiece();
	auto& BlackRook = CreatePiece();
	auto& BlackKnight = CreatePiece();
	auto& BlackBishop = CreatePiece();
	auto& BlackKing = CreatePiece();
	auto& BlackPawn = CreatePiece();
	auto& WhiteRook = CreatePiece();
	auto& WhiteKnight = CreatePiece();
	auto& WhiteBishop = CreatePiece();
	auto& WhiteQueen = CreatePiece();
	auto& WhiteKing = CreatePiece();
	auto& WhitePawn = CreatePiece();

	ChessBoard.SetRootComponent(&ChessBoard.AddComponent<ModelComponent>(ChessBoardModelPtr));
	BlackQueen.SetRootComponent(&BlackQueen.AddComponent<ModelComponent>(BlackQueenModelPtr));
	BlackRook.SetRootComponent(&BlackRook.AddComponent<ModelComponent>(BlackRookModelPtr));
	BlackKnight.SetRootComponent(&BlackKnight.AddComponent<ModelComponent>(BlackKnightModelPtr));
	BlackBishop.SetRootComponent(&BlackBishop.AddComponent<ModelComponent>(BlackBishopModelPtr));
	BlackKing.SetRootComponent(&BlackKing.AddComponent<ModelComponent>(BlackKingModelPtr));
	BlackPawn.SetRootComponent(&BlackPawn.AddComponent<ModelComponent>(BlackPawnModelPtr));
	WhiteRook.SetRootComponent(&WhiteRook.AddComponent<ModelComponent>(WhiteRookModelPtr));
	WhiteKnight.SetRootComponent(&WhiteKnight.AddComponent<ModelComponent>(WhiteKnightModelPtr));
	WhiteBishop.SetRootComponent(&WhiteBishop.AddComponent<ModelComponent>(WhiteBishopModelPtr));
	WhiteQueen.SetRootComponent(&WhiteQueen.AddComponent<ModelComponent>(WhiteQueenModelPtr));
	WhiteKing.SetRootComponent(&WhiteKing.AddComponent<ModelComponent>(WhiteKingModelPtr));
	WhitePawn.SetRootComponent(&WhitePawn.AddComponent<ModelComponent>(WhitePawnModelPtr));

	Math::Transform model;

	for (int i = 0; i < Pieces.size(); ++i)
	{
		Pieces[i]->SetLocalTransform(model);
	}

	// Light Initialize
	DirectionalLights.emplace_back(DirectionalLight(Math::Vec3(0.0f, -1.5f, 1.0f), Math::Vec3(0.922f, 0.784f, 0.314f), 4.0f));
	//for (int i = 0; i < 4; ++i)
	//{
	//	PointLights.emplace_back(PointLight(pointLightPositions[i], glm::vec3(1.0f, 0.97f, 0.92f), 10.f, 0.0f));
	//}
}
