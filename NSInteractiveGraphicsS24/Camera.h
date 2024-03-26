#pragma once
#include <glm/glm.hpp>

class Camera {
private:
	glm::mat4 referenceFrame; //tracks the position
	glm::mat4 lookFrame; //tracks the orientation
	float moveSpeed = 10.0f;

public:

	inline Camera() : referenceFrame(1), lookFrame(1) {}

	const glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);
	
	inline const auto& GetReferenceFrame() const { return referenceFrame; }
	inline void SetReferenceFrame(const glm::mat4& referenceFrame) { this->referenceFrame = referenceFrame; }

	inline const glm::mat4& GetLookFrame() const { return lookFrame; }	
	inline void SetLookFrame(const glm::mat4& lookFrame) { this->lookFrame = lookFrame; }

	inline float GetMoveSpeed() const { return moveSpeed; }
	inline void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	
	void MoveX(float delta, int direction = 1);
	void MoveY(float delta, int direction = 1);
	void MoveZ(float delta, int direction = 1);

	glm::mat4 GetView() const;
};
