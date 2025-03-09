#pragma once

#include "Ray.h"
#include <glm/glm.hpp>
#include <memory>

namespace Cicada {

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

	inline static Camera* s_mainCam;
	
	//TODO: re-evaluate the need for UBO bullshit in the camera class
	//unsigned int m_uboId = 0;
	//unsigned int m_uboBindPoint = 1; //somehow this needs to be one or a random object will be excluded from the scene

public:
	Camera(float fov, float nearClip, float farClip, float aspectRatio);
	~Camera();

	void Update() const;

	inline void SetMainCam() {
		s_mainCam = this;
	}

	static inline Camera* GetMainCam() {
		return s_mainCam;
	}

	const glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);
	
	inline const auto& GetLocalTransform() const { return m_localTransform; }
	inline void SetLocalTransform(const glm::mat4& localTransform) { m_localTransform = localTransform; }

	inline float GetMoveSpeed_OLD() const { return m_temp_moveSpeed; }
	inline void SetMoveSpeed_OLD(float moveSpeed) { m_temp_moveSpeed = moveSpeed; }
	
	void MoveX_OLD(float delta, int direction = 1);
	void MoveY_OLD(float delta, int direction = 1);
	void MoveZ_OLD(float delta, int direction = 1);

	Ray GetMouseRay(float screenPosX, float screenPosY) const;
};

}
