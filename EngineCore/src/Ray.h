#pragma once

#include "GeometricPlane.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace Cicada {

class GameObject;
class BoundingBox;

struct RaycastHit {
	float offset = -INFINITY; //negative inf signifies no intersection
	//TODO: get a reference to the object hit
};

struct Ray {

	glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, 1.0f };

	inline glm::vec3 Project(float offset) const {
		return origin + (offset * direction);
	}

	Intersection GetIntersection(const GeometricPlane& plane) const;
	Intersection GetIntersection(const BoundingBox& boundingBox) const;

	//Moller-Trumbore Intersection Algorithm
	inline bool GetIntersection(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, RaycastHit& out_hit) {
		
		constexpr float epsilon = std::numeric_limits<float>::epsilon();

		auto edge1 = v2 - v1;
		auto edge2 = v3 - v1;

		auto dxe2 = glm::cross(direction, edge2);
		float det = glm::dot(edge1, dxe2);

		if (det > -epsilon && det < epsilon)
			return false;

		float invDet = 1.0f / det;
		auto s = origin - v1;
		float u = invDet * glm::dot(s, dxe2);

		if (u < 0 || u > 1)
			return false;

		auto sxe1 = glm::cross(s, edge1);
		float v = invDet * glm::dot(direction, sxe1);

		if (v < 0 || u + v > 1)
			return false;

		float t = invDet * glm::dot(edge2, sxe1);
		if (t > epsilon) { //ray does intersect
			out_hit.offset = t;
			return true;
		}
		else //ray does not intersect, but line does
			return false;
	}

	bool IsIntersectingObject(const GameObject& object) const;
};

}
