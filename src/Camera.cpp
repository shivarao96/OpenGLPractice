#include "Camera.h"
#include <glfw3.h>
#include <iostream>

CameraObject::Camera::Camera(
	glm::vec3 pos,
	glm::vec3 worldUp
) {
	m_Position = pos;
	m_WorldUp = worldUp;
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	updateCameraVectors();
}
CameraObject::Camera::Camera(
	float posX,
	float posY,
	float posZ,
	float worldUpX,
	float worldUpY,
	float worldUpZ
) {
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(worldUpX, worldUpY, worldUpZ);
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	updateCameraVectors();
}
void CameraObject::Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = m_Speed * deltaTime;
	if (direction == CameraMovement::FORWARD) {
		m_Position += m_Front * velocity;
	}
	if (direction == CameraMovement::BACKWARD) {
		m_Position -= m_Front * velocity;
	}
	if (direction == CameraMovement::LEFT) {
		m_Position += m_Right * velocity;
	}
	if (direction == CameraMovement::RIGHT) {
		m_Position -= m_Right * velocity;
	}
	if (direction == CameraMovement::UP) {
		m_Position += m_Up * velocity;
	}
	if (direction == CameraMovement::DOWN) {
		m_Position -= m_Up * velocity;
	}
}
void CameraObject::Camera::processMouseMovement(float xOffSet, float yOffSet, bool constrainPitch = true) {
	xOffSet *= m_Sensitivity;
	yOffSet *= m_Sensitivity;

	m_Yaw -= xOffSet;
	m_Pitch += yOffSet;

	if (constrainPitch) {
		if (m_Pitch > 89.0f) {
			m_Pitch = 89.0f;
		}
		if (m_Pitch < -89.0f) {
			m_Pitch = -89.0f;
		}
	}
	updateCameraVectors();
}
void CameraObject::Camera::processMouseScroll(float yOffSet) {
	m_Zoom = -yOffSet;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > m_cZoom)
		m_Zoom = m_cZoom;
}
void CameraObject::Camera::setYaw(float val) {
	m_Yaw = val;
	updateCameraVectors();
}
void CameraObject::Camera::setPitch(float val) {
	m_Pitch = val;
	updateCameraVectors();
}
void CameraObject::Camera::setSpeed(float val) {
	m_Speed = val;
}
void CameraObject::Camera::setSensitivity(float val) {
	m_Sensitivity = val;
}
void CameraObject::Camera::setZoom(float val) {
	m_cZoom = val;
}
void CameraObject::Camera::setPosition(glm::vec3 val) {
	m_Position = val;
}
void CameraObject::Camera::setWorldUp(glm::vec3 val) {
	m_WorldUp = val;
}
glm::mat4 CameraObject::Camera::getViewMatrix() {
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
void CameraObject::Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * acosl(glm::radians(m_Pitch));
	m_Front = front;
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Front, m_Right));
}

float CameraObject::Camera::getZoomVal() const {
	return m_cZoom;
}
float CameraObject::Camera::getSpeed() const {
	return m_Speed;
}
const glm::vec3 CameraObject::Camera::getPosition() const {
	return m_Position;
}
const glm::vec3 CameraObject::Camera::getFront() const {
	return m_Front;
}