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
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right{};
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_mouseSensitivity;
	float m_movementSpeed;
	float m_zoom;
};