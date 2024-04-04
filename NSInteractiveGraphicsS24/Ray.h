#pragma once
#include <array>
#include <corecrt_math.h>
#include <ext/matrix_float4x4.hpp>
#include <ext/vector_float3.hpp>

#include "BoundingBox.h"
#include "Intersection.h"

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction; //always normalized

	Ray(const glm::vec3& origin, const glm::vec3& direction);
	Ray(float screenX, float screenY, const glm::mat4& proj, const glm::mat4& view);

	glm::vec3 GetPositionAlong(float offset) const;
	Intersection GetIntersection(const BoundingPlane& plane) const;
	std::array<Intersection,6> GetIntersection(const BoundingBox& boundingBox) const;
};
