#include "Piece.h"
#include "Scene/Components/ModelComponent.h"


void Piece::Initialize()
{

}

void Piece::Update(float dt)
{}

void Piece::Uninitialize()
{}


glm::mat4 Piece::GetLocalTransform() const
{
	SceneComponent* sceneComponent = dynamic_cast<SceneComponent*>(RootComponent);
	if (!sceneComponent)
	{
		return glm::mat4(1.0f);
	}
	return sceneComponent->GetLocalTransform();
}

void Piece::SetLocalTransform(const glm::mat4& transform)
{
	SceneComponent* sceneComponent = dynamic_cast<SceneComponent*>(RootComponent);
	if (!sceneComponent)
	{
		return;
	}
	sceneComponent->SetLocalTransform(transform);
}


