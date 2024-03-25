#include "Camera.h"
#include <ext/matrix_transform.hpp>

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

glm::mat4 Camera::LookForward() const {
	glm::vec3 camPos = referenceFrame[3];
	glm::vec3 camForward = -lookFrame[2]; //negative z axis of the camera's local coordinate system
	glm::vec3 camTarget = camPos + camForward;
	glm::vec3 camUp = lookFrame[1];
	return glm::lookAt(camPos, camTarget, camUp);
}

glm::mat4 Camera::LookAt(const glm::vec3& point) const {
	glm::vec3 camPos = referenceFrame[3];
	glm::vec3 camUp = lookFrame[1];
	return glm::lookAt(camPos, point, camUp);
}
