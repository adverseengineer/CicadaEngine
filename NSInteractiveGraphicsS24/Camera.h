#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Camera {
friend class GraphicsEnvironment;
private:
	float fov = 60.0f;
	float nearClip = 0.01f;
	float farClip = 200.0f;

	glm::mat4 referenceFrame; //tracks the position
	glm::mat4 lookFrame; //tracks the orientation
	
	//glm::mat4 view; //= ref * look
	glm::mat4 projection;

	float moveSpeed = 10.0f;

public:
	Camera(float fov, float nearClip, float farClip, float aspectRatio);

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
	Ray GetMouseRay(float screenPosX, float screenPosY) const;
};
