#pragma once
#include "SceneComponent.h"
class PrimitiveComponent :
    public SceneComponent
{
public:
	PrimitiveComponent(Piece* piece) : SceneComponent(piece) {}

	bool IsVisible() const { return bVisible; }
	bool IsCastShadow() const { return bCastShadow; }

private:
	bool bVisible = true;
	bool bCastShadow = true;

};

