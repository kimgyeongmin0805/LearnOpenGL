#ifndef CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	UP,
	DOWN,
};

// defualt value
const glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float SPEED = 2.5f;
const float FOV = 45.0f;


class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float Sensitivity;
	float Speed;
	float Fov;

	Camera(glm::vec3 position = POSITION)
		: Position(position), Front(FRONT), Up(UP), Right(RIGHT), WorldUp(WORLDUP), Yaw(YAW), Pitch(PITCH), Sensitivity(SENSITIVITY), Speed(SPEED), Fov(FOV) {
	}

	glm::mat4 GetProjectionMatrix() {
		return glm::perspective(glm::radians(Fov), 800.0f / 600.0f, 0.1f, 100.0f);
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}
	
	void ProcessKeyboard(CameraMovement direction, float deltaTime) {
		float velocity = Speed * deltaTime;
		if (direction == CameraMovement::FORWARD)
			Position += Front * velocity;
		if (direction == CameraMovement::BACKWARD)
			Position -= Front * velocity;
		if (direction == CameraMovement::RIGHT)
			Position += Right * velocity;
		if (direction == CameraMovement::LEFT)
			Position -= Right * velocity;
		if (direction == CameraMovement::UP)
			Position += Up * velocity;
		if (direction == CameraMovement::DOWN)
			Position -= Up * velocity;
	}

	void ProcessCursorPos(float xoffset, float yoffset) {
		xoffset *= Sensitivity;
		yoffset *= Sensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void ProcessScroll(float yoffset) {
		Fov -= yoffset;
		
		if (Fov < 1.0f)
			Fov = 1.0f;
		if (Fov > 45.0f)
			Fov = 45.0f;
	}
};

#endif