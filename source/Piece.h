#pragma once
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Model.h"

class Piece
{
public:
	glm::mat4 Transform;
	std::shared_ptr<Model> ModelPtr;
};

