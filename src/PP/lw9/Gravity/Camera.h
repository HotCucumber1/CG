#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();

	glm::mat4 GetViewMatrix() const;

	glm::mat4 GetProjectionMatrix(float aspectRatio) const;

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	void ProcessMouseScroll(float yOffset);

	void ProcessKeyboard(int key, float deltaTime);

	glm::vec3 GetPosition() const
	{
		return m_position;
	}
	void SetPosition(const glm::vec3& position)
	{
		m_position = position;
	}

private:
	void UpdateCameraVectors();

private:
	glm::vec3 m_position = {0, 0, 20};
	glm::vec3 m_front = {0.0f, 0.0f, -1.0f};
	glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
	glm::vec3 m_right{};
	glm::vec3 m_worldUp = {0.0f, 1.0f, 0.0f};

	float m_yaw = -90;
	float m_pitch = 0;
	float m_mouseSensitivity = 0.1;
	float m_movementSpeed = 15;
	float m_zoom = 45;
};