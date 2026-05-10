#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
	: Pos(glm::vec3(0.0f, 0.0f, 0.0f))
	, Pitch(0.0f)
	, Yaw(-90.0f)
	, WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, MoveSpeed(2.5f)
	, RotateSpeed(0.05f)
	, ZoomSpeed(1.0f)
	, Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, Right(glm::vec3(1.0f, 0.0f, 0.0f))
	, Up(WorldUp)
	, FoV(45.0f)
	, PitchUpperBounds(89.0f)
	, PitchLowerBounds(-89.0f)
	, FoVUpperBounds(45.0f)
	, FoVLowerBounds(1.0f)
{
}

void Camera::UpdateAxis()
{
	float yawRad = glm::radians(Yaw);
	float pitchRad = glm::radians(Pitch);

	glm::vec3 scaledFront;
	scaledFront.x = cos(yawRad) * cos(pitchRad);
	scaledFront.y = sin(pitchRad);
	scaledFront.z = sin(yawRad) * cos(pitchRad);

	Front = glm::normalize(scaledFront);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::LookAt()
{
	glm::mat4 rotation;
	glm::mat4 translation;
	// 1. Position = known
	// 2. Calculate cameraDirection
	glm::vec3 zaxis = glm::normalize(Pos - (Pos + Front));
	// 3. Get positive right axis vector
	glm::vec3 xaxis = Right;
	// 4. Calculate camera up vector
	glm::vec3 yaxis = Up;

	rotation[0][0] = xaxis.x; rotation[1][0] = xaxis.y; rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; rotation[1][1] = yaxis.y; rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; rotation[1][2] = zaxis.y; rotation[2][2] = zaxis.z;

	translation[3][0] = -Pos.x; translation[3][1] = -Pos.y; translation[3][2] = -Pos.z;
	return rotation * translation;
}

void Camera::Move(CameraMovement direction, float deltaTime)
{
	// 禁止飞行时使用MoveFront
	glm::vec3 MoveFront = Front;
	MoveFront.y = 0;
	MoveFront = glm::normalize(MoveFront);

	switch (direction)
	{
	case FORWARD:
		Pos += deltaTime * MoveSpeed * Front;
		break;
	case BACKWARD:
		Pos += -deltaTime * MoveSpeed * Front;
		break;
	case LEFT:
		Pos += -deltaTime * MoveSpeed * Right;
		break;
	case RIGHT:
		Pos += deltaTime * MoveSpeed * Right;
		break;
	default:
		break;
	}
}

void Camera::Rotate(float offsetX, float offsetY)
{
	offsetX *= RotateSpeed;
	offsetY *= RotateSpeed;

	Yaw += offsetX;
	Pitch += offsetY;

	Pitch = glm::clamp(Pitch, PitchLowerBounds, PitchUpperBounds);

	UpdateAxis();
}

void Camera::Zoom(float value)
{
	FoV += value * ZoomSpeed;
	FoV = glm::clamp(FoV, FoVLowerBounds, FoVUpperBounds);
}

glm::mat4 Camera::GetViewMatrix()
{
	return LookAt();
}
