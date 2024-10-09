#include "Camera.h"

#include <ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

Camera::Camera(float fov, float nearClip, float farClip, float aspectRatio) :
	m_fov(fov), m_nearClip(nearClip), m_farClip(farClip), m_aspectRatio(aspectRatio),
	m_localTransform(1.0f) {
	m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

	glGenBuffers(1, &m_uboId);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
}

Camera::~Camera() {
	glDeleteBuffers(1, &m_uboId);
}

void Camera::Update() const {
	
	glm::mat4 view = glm::inverse(m_localTransform);

	//bind the UBO and send the view and projection one after another
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(view), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(view), sizeof(m_projection), glm::value_ptr(m_projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //remember to unbind it
}

const glm::vec3 Camera::GetPosition() const {
	return glm::vec3(m_localTransform[3]);
}

void Camera::SetPosition(const glm::vec3& position) {
	m_localTransform[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveX(float delta, int direction) {
	m_localTransform[3] -= direction * m_temp_moveSpeed * delta * m_localTransform[0];
}

void Camera::MoveY(float delta, int direction) {
	m_localTransform[3] -= direction * m_temp_moveSpeed * delta * m_localTransform[1];
}

void Camera::MoveZ(float delta, int direction) {
	m_localTransform[3] -= direction * m_temp_moveSpeed * delta * m_localTransform[2];
}

Ray Camera::GetMouseRay(float screenPosX, float screenPosY) const {

	Ray ray;

	glm::mat4 projInverse = glm::inverse(m_projection);
	glm::mat4 viewInverse = m_localTransform;

	glm::vec4 rayDirClip = glm::vec4(screenPosX, screenPosY, -1.0f, 1.0f);
	glm::vec4 rayDirE = projInverse * rayDirClip;
	rayDirE.z = -1.0f; // Forward is down -ve z
	rayDirE.w = 0.0f;  // A direction vector 
	glm::vec4 rayDirW = glm::normalize(viewInverse * rayDirE);
	ray.direction = rayDirW;

	glm::vec4 rayStartS = glm::vec4(screenPosX, screenPosY, 1.0f, 1.0f);
	glm::vec4 rayStartE = projInverse * rayStartS;
	rayStartE.z = 1.0f;
	rayStartE.w = 1.0f;
	glm::vec4 rayStartW = viewInverse * rayStartE;
	ray.origin = glm::vec3(rayStartW);

	return ray;
	/*
	Ray ray;

	glm::mat4 projInverse = glm::inverse(projection);
	glm::mat4 viewInverse = glm::inverse(GetView());

	glm::vec4 tempOrigin = glm::vec4(screenPosX, screenPosY, 1.0f, 1.0f); //start with point in screen space
	tempOrigin = projInverse * tempOrigin; //take the point from screen to camera space
	tempOrigin.z = 1.0f;
	tempOrigin.w = 1.0f; //points have one for their homogenous coord
	tempOrigin = viewInverse * tempOrigin; //take the point from camera to world space
	ray.origin = glm::vec3(tempOrigin);

	glm::vec4 tempDir = glm::vec4(screenPosX, screenPosY, -1.0f, 1.0f); //start with vector in screen space
	tempDir = projInverse * tempDir; //take the vector from screen to camera space
	tempDir.z = -1.0f;
	tempDir.w = 0.0f; //vectors have zero for their homogenous coord
	tempDir = glm::normalize(viewInverse * tempDir); //take the vector from camera space to world space
	ray.direction = tempDir;

	return ray;
	*/
};