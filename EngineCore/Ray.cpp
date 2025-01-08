#include "Ray.h"
#include "GameObject.h"
#include "BoundingBox.h"

Intersection Ray::GetIntersection(const GeometricPlane& plane) const {

	Intersection intersection;
	// Vince (2014) pp. 200
	float normDotDir = glm::dot(plane.normal, direction);
	if (normDotDir == 0) return intersection; // parallel
	float normDotOrigin = glm::dot(plane.normal, origin);
	float lambda = -(normDotOrigin + plane.distanceFromOrigin) / normDotDir;
	intersection.offset = lambda;
	if (lambda < 0) return intersection; // miss, behind view point
	intersection.isIntersecting = true;
	return intersection;
}

Intersection Ray::GetIntersection(const BoundingBox& boundingBox) const {
	return Intersection();
}

bool Ray::IsIntersectingObject(const GameObject& object) const {
	if (!object.HasBoundingBox()) return false;
	return object.GetBoundingBox()->IsIntersectingWithRay(*this);
}

