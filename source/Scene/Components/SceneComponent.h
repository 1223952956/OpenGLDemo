#pragma once
#include "PieceComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class SceneComponent :
    public PieceComponent
{
public:
	SceneComponent(Piece* piece) : PieceComponent(piece), Transform(1.0f) {}

	void AttachToComponent(SceneComponent* parent);
	SceneComponent* GetParent() const { return Parent; }
	const std::vector<SceneComponent*>& GetChildren() const { return Children; }

	void SetLocalTransform(const glm::mat4& transform) { Transform = transform; }
	glm::mat4 GetLocalTransform() const { return Transform; }

	glm::mat4 GetWorldTransform() const;


private:
	SceneComponent* Parent = nullptr;
	std::vector<SceneComponent*> Children;
	glm::mat4 Transform;

	void AddChild(SceneComponent* child);
	void RemoveChild(SceneComponent* child);

};

