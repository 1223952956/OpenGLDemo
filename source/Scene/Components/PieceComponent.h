#pragma once

class Piece;

class PieceComponent
{
public:
	PieceComponent(Piece* piece) : owner(piece) {}
	virtual ~PieceComponent() = default;

	virtual void Update(float dt) {};

	Piece* GetOwner() const { return owner; }

private:
	Piece* owner;

};

