#pragma once
#include "SceneComponent.h"
class PrimitiveComponent :
    public SceneComponent
{
public:
	PrimitiveComponent(Piece* piece) : SceneComponent(piece) {}
	PrimitiveComponent(Piece* piece, UUID id) : SceneComponent(piece, id) {}

	bool IsVisible() const { return bVisible; }
	bool IsCastShadow() const { return bCastShadow; }

	void SetVisible(bool visible) { bVisible = visible;}
	void SetCastShadow(bool castShadow) { bCastShadow = castShadow; }

private:
	bool bVisible = true;
	bool bCastShadow = true;

};

