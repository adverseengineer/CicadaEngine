#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GeometricLine.h"
#include "GeometricPlane.h"
#include <vector>

class GraphicsObject;
class BoundingBox;

struct Ray {

	glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, 1.0f }; //always normalized

	inline Ray() = default;
	Ray(float screenX, float screenY, const glm::mat4& proj, const glm::mat4& view);
	inline ~Ray() = default;

	inline glm::vec3 GetPosition(float offset) const {
		return origin + (offset * direction);
	}

	Intersection GetIntersectionWithPlane(const GeometricPlane& plane) const;
	Intersection GetIntersectionWithBoundingBox(const BoundingBox& boundingBox) const;
	bool IsIntersectingObject(const GraphicsObject& object) const;
	//bool IsIntersectingObject(const std::shared_ptr<GraphicsObject>& object) const;
	//bool IsPointAlongRay(glm::vec3 point) const;
};
