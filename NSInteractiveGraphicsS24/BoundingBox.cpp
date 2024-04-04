#include "BoundingBox.h"

BoundingBox::BoundingBox(float width, float height, float depth, const glm::mat4& frame) :
	width(width), height(height), depth(depth), frame(frame) {
	float hw = this->width / 2;
	float hh = this->height / 2;
	float hd = this->depth / 2;
	glm::vec3 xAxis = frame[0];
	glm::vec3 yAxis = frame[1];
	glm::vec3 zAxis = frame[2];
	invFrame = glm::inverse(frame);
	planes[FRONT] = { -zAxis, hd };
	planes[BACK] = { zAxis, hd };
	planes[RIGHT] = { -xAxis, hw };
	planes[LEFT] = { xAxis, hw };
	planes[TOP] = { -yAxis, hh };
	planes[BOTTOM] = { yAxis, hh };
}
