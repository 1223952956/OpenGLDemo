#pragma once
#include "PieceComponent.h"
#include <vector>
#include "Math/Transform.h"

class SceneComponent :
    public PieceComponent
{
public:
	SceneComponent(Piece* piece) : PieceComponent(piece) {}

	void AttachToComponent(SceneComponent* parent);
	SceneComponent* GetParent() const { return Parent; }
	const std::vector<SceneComponent*>& GetChildren() const { return Children; }

	void SetLocalTransform(const Math::Transform& transform) { Transform = transform; }
	Math::Transform GetLocalTransform() const { return Transform; }

	Math::Transform GetWorldTransform() const;


private:
	SceneComponent* Parent = nullptr;
	std::vector<SceneComponent*> Children;
	Math::Transform Transform;

	void AddChild(SceneComponent* child);
	void RemoveChild(SceneComponent* child);

};

