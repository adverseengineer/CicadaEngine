#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Camera {
friend class GraphicsEnvironment;
private:
	float m_fov = 60.0f;
	float m_nearClip = 0.01f;
	float m_farClip = 200.0f;

	float m_aspectRatio;

	glm::mat4 m_localTransform;
	glm::mat4 m_projection;

	float m_temp_moveSpeed = 10.0f;

	unsigned int m_uboId = 0;

public:
	Camera(float fov, float nearClip, float farClip, float aspectRatio);
	~Camera();

	void Update() const;

	const glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);
	
	inline const auto& GetLocalTransform() const { return m_localTransform; }
	inline void SetLocalTransform(const glm::mat4& localTransform) { m_localTransform = localTransform; }

	inline float GetMoveSpeed() const { return m_temp_moveSpeed; }
	inline void SetMoveSpeed(float moveSpeed) { m_temp_moveSpeed = moveSpeed; }
	
	void MoveX(float delta, int direction = 1);
	void MoveY(float delta, int direction = 1);
	void MoveZ(float delta, int direction = 1);

	Ray GetMouseRay(float screenPosX, float screenPosY) const;
};
