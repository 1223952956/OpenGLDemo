#pragma once
#include <vector>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "Scene/Components/PieceComponent.h"

class Piece
{
public:
	void Initialize();
	void Update(float dt);
	void Uninitialize();

	PieceComponent* GetRootComponent() { return RootComponent; }
	void SetRootComponent(PieceComponent* component) { RootComponent = component; }

	template<class T, class... Args>
	T& AddComponent(Args&&... args);
	template<class T>
	T* GetComponent();
	template<class T>
	std::vector<T*> GetComponents();

	glm::mat4 GetLocalTransform() const;
	void SetLocalTransform(const glm::mat4& transform);

private:
	std::vector<std::unique_ptr<PieceComponent>> Components;
	PieceComponent* RootComponent = nullptr;

};

template<class T, class ...Args>
T& Piece::AddComponent(Args && ...args)
{
	static_assert(std::is_base_of_v<PieceComponent, T>, "T must be derived from PieceComponent");

	std::unique_ptr<T> component = std::make_unique<T>(this, std::forward<Args>(args)...);
	T* result = component.get();
	
	Components.push_back(std::move(component));

	return *result;
}

template<class T>
T* Piece::GetComponent()
{
	for (const auto& component : Components)
	{
		if (auto* result = dynamic_cast<T*>(component.get()))
		{
			return result;
		}
	}
	return nullptr;
}

template<class T>
std::vector<T*> Piece::GetComponents()
{
	std::vector<T*> result;
	for (const auto& component : Components)
	{
		if (auto* casted = dynamic_cast<T*>(component.get()))
		{
			result.push_back(casted);
		}
	}
	return result;
}
