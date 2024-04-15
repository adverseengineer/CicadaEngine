#include "Camera.h"
#include <ext/matrix_transform.hpp>

Camera::Camera(float fov, float nearClip, float farClip, float aspectRatio) :
	fov(fov), nearClip(nearClip), farClip(farClip), referenceFrame(1.0f), lookFrame(1.0f) {
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

const glm::vec3 Camera::GetPosition() const {
	return glm::vec3(referenceFrame[3]);
}

void Camera::SetPosition(const glm::vec3& position) {
	referenceFrame[3] = glm::vec4(position, 1.0f);
}

void Camera::MoveX(float delta, int direction) {
	referenceFrame[3] -= direction * moveSpeed * delta * lookFrame[0];
}

void Camera::MoveY(float delta, int direction) {
	referenceFrame[3] -= direction * moveSpeed * delta * lookFrame[1];
}

void Camera::MoveZ(float delta, int direction) {
	referenceFrame[3] -= direction * moveSpeed * delta * lookFrame[2];
}

glm::mat4 Camera::GetView() const {
	//we store the position and orientation of the camera in two separate matrices
	//we need one matrix that represents both, so multiply them
	auto fullTransform = referenceFrame * lookFrame;
	//then simply take the inverse so that multiplying by the view converts from world to camera space
	return glm::inverse(fullTransform);
}

Ray Camera::GetMouseRay(float screenPosX, float screenPosY) const {

	Ray ray;

	glm::mat4 projInverse = glm::inverse(projection);
	glm::mat4 viewInverse = glm::inverse(GetView());

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