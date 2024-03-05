#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	glm::mat4 referenceFrame;
	glm::mat4 lookFrame;
	float moveSpeed = 10.0f;
public:

	inline Camera(void) : referenceFrame(1), lookFrame(1) {}

	inline void SetLookFrame(const glm::mat4& lookFrame) { this->lookFrame = lookFrame; }
	inline void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	inline void SetPosition(const glm::vec3& position) { referenceFrame[3] = glm::vec4(position, 1.0f); }
	inline glm::vec3 GetPosition(void) const { return referenceFrame[3]; }

	glm::mat4 LookForward(void) const;
	glm::mat4 LookAt(const glm::vec3& point) const;

	inline void MoveX(float delta, int direction = 1) { referenceFrame[3] -= direction * moveSpeed * delta * lookFrame[0]; }
	inline void MoveY(float delta, int direction = 1) { referenceFrame[3] -= direction * moveSpeed * delta * lookFrame[1]; }
	inline void MoveZ(float delta, int direction = 1) { referenceFrame[3] -= direction * moveSpeed * delta * lookFrame[2]; }
	
};
