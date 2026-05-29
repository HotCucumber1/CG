#include "Camera.h"
#include <GLFW/glfw3.h>

Camera::Camera()
{
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = std::cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = std::sin(glm::radians(m_pitch));
	front.z = std::sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix(const float aspectRatio) const
{
	return glm::perspective(glm::radians(m_zoom), aspectRatio, 0.1f, 1000.0f);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, const bool constrainPitch)
{
	static constexpr float pitchMax = 89;
	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (constrainPitch)
	{
		if (m_pitch > pitchMax)
		{
			m_pitch = pitchMax;
		}
		if (m_pitch < -pitchMax)
		{
			m_pitch = -pitchMax;
		}
	}
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	static constexpr float zoomMin = 1;
	m_zoom -= yOffset;
	if (m_zoom < zoomMin)
	{
		m_zoom = zoomMin;
	}
	if (m_zoom > 90 - zoomMin)
	{
		m_zoom = 90 - zoomMin;
	}
}

void Camera::ProcessKeyboard(const int key, const float deltaTime)
{
	const auto velocity = m_movementSpeed * deltaTime;

	if (key == GLFW_KEY_W)
	{
		m_position += m_front * velocity;
	}
	if (key == GLFW_KEY_S)
	{
		m_position -= m_front * velocity;
	}
	if (key == GLFW_KEY_A)
	{
		m_position -= m_right * velocity;
	}
	if (key == GLFW_KEY_D)
	{
		m_position += m_right * velocity;
	}
	if (key == GLFW_KEY_Q)
	{
		m_position -= m_up * velocity;
	}
	if (key == GLFW_KEY_E)
	{
		m_position += m_up * velocity;
	}
}
