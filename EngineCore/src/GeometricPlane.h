#pragma once

#include <glm/glm.hpp>

namespace Cicada {

class GeometricLine;

// Vince, J. (2014). Mathematics for Computer Graphics, Springer

struct Intersection{
	bool isIntersecting = false;
	float offset = -1.0f;
};

struct GeometricPlane {
	glm::vec3 normal;
	float distanceFromOrigin;

	void SetNormal(glm::vec3 normal) { this->normal = normal; }
	void SetDistanceFromOrigin(float distance){ 
		distanceFromOrigin = fabs(distance);
	}
	void Set(glm::vec3 normal, float distance) {
		SetNormal(normal);
		SetDistanceFromOrigin(distance);
	}
};

}
