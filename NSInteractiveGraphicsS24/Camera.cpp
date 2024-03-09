#include "Camera.h"
#include <ext/matrix_transform.hpp>

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
