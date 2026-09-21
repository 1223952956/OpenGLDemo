#pragma once
#include "PrimitiveComponent.h"
#include "Renderer/Model.h"

class ModelComponent :
    public PrimitiveComponent
{
public:
	ModelComponent(Piece* piece, std::shared_ptr<Model> model) 
		: PrimitiveComponent(piece), ModelPtr(std::move(model)) {}

	void SetMesh(std::shared_ptr<Model> model) { ModelPtr = model; }

	Model* GetModel() const { return ModelPtr.get(); }
private:
	std::shared_ptr<Model> ModelPtr;
};

