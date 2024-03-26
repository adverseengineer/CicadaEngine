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

glm::mat4 Camera::GetView() const {
	//we store the position and orientation of the camera in two separate matrices
	//we need one matrix that represents both, so multiply them
	auto fullTransform = referenceFrame * lookFrame;
	//then simply take the inverse so that multiplying by the view converts from world to camera space
	return glm::inverse(fullTransform);
}
