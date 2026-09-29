#pragma once
#include "PrimitiveComponent.h"
#include "Renderer/Model.h"

class ModelComponent :
    public PrimitiveComponent
{
public:
	ModelComponent(Piece* piece, std::shared_ptr<Model> model) 
		: PrimitiveComponent(piece), ModelPtr(std::move(model)) {}
	ModelComponent(Piece* piece, std::shared_ptr<Model> model, UUID id)
		: PrimitiveComponent(piece, id), ModelPtr(std::move(model)) {}
	ModelComponent(Piece* piece, UUID id) : PrimitiveComponent(piece, id) {}

	void SetMesh(std::shared_ptr<Model> model) { ModelPtr = model; }

	Model* GetModel() const { return ModelPtr.get(); }
private:
	std::shared_ptr<Model> ModelPtr;
};

