#include "Scene.h"

void Scene::Update(float deltaTime)
{

}

Piece& Scene::CreatePiece()
{
    auto piece = std::make_unique<Piece>();

    Piece& ref = *piece;

    Pieces.push_back(std::move(piece));

    return ref;
}
