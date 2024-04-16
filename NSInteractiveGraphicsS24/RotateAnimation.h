#pragma once
#include "AnimationBehavior.h"
#include "glm/glm.hpp"

class RotateAnimation : public AnimationBehavior {

private:
	glm::vec3 axis;
	float speed;

public:
	inline RotateAnimation(glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f), float speed = 90.0f)
	: AnimationBehavior(), axis(axis), speed(speed) {
	}
	inline ~RotateAnimation() = default;

	void Update(double elapsedSeconds);
};
