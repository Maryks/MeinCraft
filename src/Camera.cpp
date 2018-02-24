#include "Camera.h"

#include <iostream>

Camera::Camera(int windowWidth, int windowHeight)
	: m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVITY),
	m_callZoom(ZOOM_MAX),
	m_callPosition(glm::vec3(0.f, 64.f, 3.f)),
	m_callFront(glm::vec3(0.f, 0.f, -1.f)),
	m_worldUp(glm::vec3(0.f, 1.f, 0.f)),
	m_yaw(YAW),
	m_pitch(PITCH),
	m_aspect(static_cast<GLfloat>(windowWidth) / static_cast<GLfloat>(windowHeight))
{
	// in order to be able to reset the camera, store the initial zoom, position, front
	m_zoom = m_callZoom;
	m_position = m_callPosition;
	m_front = m_callFront;

	this->updateCameraVectors();
	this->processMouseScroll(0.f);	// initialize projection matrix

	std::cout << "Camera created." << std::endl;
}

Camera::Camera(int windowWidth, int windowHeight, glm::vec3 position, glm::vec3 front)
	: m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVITY),
	m_callZoom(ZOOM_MAX),
	m_callPosition(position),
	m_callFront(front),
	m_worldUp(glm::vec3(0.f, 1.f, 0.f)),
	m_yaw(YAW),
	m_pitch(PITCH),
	m_aspect(static_cast<GLfloat>(windowWidth) / static_cast<GLfloat>(windowHeight))
{
	m_zoom = m_callZoom;
	m_position = m_callPosition;
	m_front = m_callFront;

	this->updateCameraVectors();
	this->processMouseScroll(0.f);	// initialize projection matrix

	std::cout << "Camera created." << std::endl;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}

glm::mat4 Camera::getProjectionViewMatrix()
{
	return (m_projection * glm::lookAt(m_position, m_position + m_front, m_up));	// make projection view matrix
}

void Camera::processKeyboardInput(Camera_Movement direction)
{
	GLfloat velocity = m_movementSpeed;
	if (direction == FORWARD)
	{
		m_position += m_front * velocity;
	}
	if (direction == BACKWARD)
	{
		m_position -= m_front * velocity;
	}
	if (direction == LEFT)
	{
		m_position -= m_right * velocity;
	}
	if (direction == RIGHT)
	{
		m_position += m_right * velocity;
	}
	if (direction == UP)
	{
		m_position += m_up * velocity;
	}
}

void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset)
{
	xOffset *= m_mouseSensitivity;
	yOffset *= m_mouseSensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (m_pitch > 89.f)
		m_pitch = 89.f;
	if (m_pitch < -89.f)
		m_pitch = -89.f;
	// avoid large yaw numbers? makes sense?
	if (m_yaw >= 360.f)
		m_yaw = 0.f;
	if (m_yaw <= -360.f)
		m_yaw = 0.f;

	this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat scrollDelta)
{
	if (m_zoom >= ZOOM_MIN && m_zoom <= m_callZoom)
		m_zoom -= scrollDelta*0.1f;
	if (m_zoom <= ZOOM_MIN)
		m_zoom = ZOOM_MIN;
	if (m_zoom >= m_callZoom)
		m_zoom = m_callZoom;

	m_projection = glm::perspective(m_zoom, m_aspect, NEAR, FAR);
}

void Camera::resetPosition()
{
	m_position = m_callPosition;
	m_front = m_callFront;
	this->resetView();
}

void Camera::resetView()
{
	m_yaw = 0.f;
	m_pitch = 0.f;
	m_zoom = m_callZoom;
}

void Camera::setPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Camera::updateCameraVectors()
{
	// compute camera front via Euler angles (but use quaternions eventually)
	glm::vec3 front;
	front.x = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = -cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
