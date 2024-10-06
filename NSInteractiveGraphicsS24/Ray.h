#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GeometricLine.h"
#include "GeometricPlane.h"
#include <vector>

class GameObject;
class BoundingBox;

struct Ray {

	glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, 1.0f };

	Ray() = default;
	~Ray() = default;

	inline glm::vec3 Project(float offset) const {
		return origin + (offset * direction);
	}

	Intersection GetIntersection(const GeometricPlane& plane) const;
	Intersection GetIntersection(const BoundingBox& boundingBox) const;
	bool IsIntersectingObject(const GameObject& object) const;
};
