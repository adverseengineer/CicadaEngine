#pragma once
#include <ext/matrix_float4x4.hpp>
#include <vector>

#include "BoundingPlane.h"

class BoundingBox {
friend class Ray;
public:
	static const int FRONT = 0;
	static const int BACK = 1;
	static const int LEFT = 2;
	static const int RIGHT = 3;
	static const int TOP = 4;
	static const int BOTTOM = 5;

protected:
	float width = 1.0f, height = 1.0f, depth = 1.0f;
	glm::mat4 frame;
	glm::mat4 invFrame;
	BoundingPlane planes[6];

public:
	BoundingBox(float width, float height, float depth, const glm::mat4& frame);

	inline void SetReferenceFrame(const glm::mat4& frameIn) {
		frame = glm::mat4(frameIn);
		invFrame = glm::inverse(frame);
	}
	inline glm::mat4 GetReferenceFrame() const { return frame; }
};
