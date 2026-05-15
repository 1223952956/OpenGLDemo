#pragma once
#include <glm/glm.hpp>



enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera();

	glm::vec3 Pos;
	float Pitch;
	float Yaw;

	glm::vec3 WorldUp;

	float MoveSpeed;
	float RotateSpeed;
	float ZoomSpeed;

private:
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;

	float FoV;

	float PitchUpperBounds;
	float PitchLowerBounds;

	float FoVUpperBounds;
	float FoVLowerBounds;

	void UpdateAxis();
	glm::mat4 LookAt();

public:
	void Move(CameraMovement direction, float deltaTime);
	void Rotate(float offsetX, float offsetY);
	void Zoom(float value);
	glm::mat4 GetViewMatrix();
	float GetFoV() const { return FoV; }
	glm::vec3 GetFront() const { return Front; }
};

