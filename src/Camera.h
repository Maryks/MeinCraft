#ifndef CAMERA_H
#define CAMERA_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP
};

namespace {
	const GLfloat SPEED = 40.f;
	const GLfloat SENSITIVITY = 0.1f;
	const GLfloat ZOOM_MIN = 44.f;
	const GLfloat ZOOM_MAX = 45.f;
	const GLfloat YAW = 0.f;
	const GLfloat PITCH = 0.f;
	const GLfloat NEAR = 0.1f;
	const GLfloat FAR = 1000.f;
}

class Camera
{
public:
	Camera() = delete;
	Camera(int, int);
	Camera(int, int, glm::vec3 position, glm::vec3 front);

	glm::vec3 getPosition();
	glm::mat4 getProjectionViewMatrix();
	void processKeyboardInput(Camera_Movement direction, GLfloat deltaTime);
	void processMouseMovement(GLfloat xOffset, GLfloat yOffset);
	void processMouseScroll(GLfloat scrollDelta);
	void resetPosition();
	void resetView();
	void setPosition(glm::vec3);

	GLfloat m_zoom;	// I'm too lazy to implement getZoom()/setZoom(GLfloat)

private:
	void updateCameraVectors();

	GLfloat m_aspect;
	GLfloat m_movementSpeed;
	GLfloat m_mouseSensitivity;
	GLfloat m_yaw;
	GLfloat m_pitch;
	const GLfloat m_callZoom;

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	const glm::vec3 m_worldUp;
	const glm::vec3 m_callPosition;
	const glm::vec3 m_callFront;

	glm::mat4 m_projection;	// projection matrix
};

#endif // !CAMERA_H
