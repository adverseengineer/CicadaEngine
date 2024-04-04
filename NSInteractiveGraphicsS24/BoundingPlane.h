#pragma once
#include <corecrt_math.h>
#include <ext/vector_float3.hpp>

struct BoundingPlane {

	glm::vec3 normal;
	float distanceFromOrigin;

	inline void SetDistanceFromOrigin(float distance) {
		distanceFromOrigin = fabs(distance);
	}
};
