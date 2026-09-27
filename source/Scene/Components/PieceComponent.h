#pragma once
#include "UUID.h"

class Piece;

class PieceComponent
{
public:
	PieceComponent(Piece* piece) : owner(piece) {}
	PieceComponent(Piece* piece, UUID id) : owner(piece), ID(id) {}
	virtual ~PieceComponent() = default;

	virtual void Update(float dt) {};

	Piece* GetOwner() const { return owner; }
	UUID GetID() const { return ID; }

private:
	Piece* owner;

	UUID ID;

};

