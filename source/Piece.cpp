#include "Piece.h"
#include "Scene/Components/ModelComponent.h"


void Piece::Initialize()
{

}

void Piece::Update(float dt)
{}

void Piece::Uninitialize()
{}


Math::Transform Piece::GetLocalTransform() const
{
	SceneComponent* sceneComponent = dynamic_cast<SceneComponent*>(RootComponent);
	if (!sceneComponent)
	{
		return Math::Transform();
	}
	return sceneComponent->GetLocalTransform();
}

void Piece::SetLocalTransform(const Math::Transform& transform)
{
	SceneComponent* sceneComponent = dynamic_cast<SceneComponent*>(RootComponent);
	if (!sceneComponent)
	{
		return;
	}
	sceneComponent->SetLocalTransform(transform);
}

Math::Transform Piece::GetWorldTransform() const
{
	SceneComponent* sceneComponent = dynamic_cast<SceneComponent*>(RootComponent);
	if (!sceneComponent)
	{
		return Math::Transform();
	}
	return sceneComponent->GetWorldTransform();
}


