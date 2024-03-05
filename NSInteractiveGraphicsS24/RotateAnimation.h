#pragma once
#include "Animation.h"
#include "glm/glm.hpp"

class RotateAnimation : public Animation {

private:
	glm::vec3 axis;
	float speed;

public:
	inline RotateAnimation(glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f), float speed = 90.0f) : Animation(), axis(axis), speed(speed) {}

	void Update(double elapsedSeconds);
};
