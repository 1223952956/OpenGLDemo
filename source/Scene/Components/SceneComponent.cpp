#include "SceneComponent.h"
#include <algorithm>

void SceneComponent::AttachToComponent(SceneComponent* parent)
{
	if (Parent)
	{
		Parent->RemoveChild(this);
	}
	Parent = parent;
	if (Parent)
	{
		Parent->AddChild(this);
	}
}

Math::Transform SceneComponent::GetWorldTransform() const
{
	if (Parent)
	{
		return Parent->GetWorldTransform() * Transform;
	}
	else
	{
		return Transform;
	}
}


void SceneComponent::AddChild(SceneComponent* child)
{
	Children.push_back(child);
}

void SceneComponent::RemoveChild(SceneComponent* child)
{
	Children.erase(std::remove(Children.begin(), Children.end(), child), Children.end());
}
