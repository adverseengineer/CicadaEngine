#include "Ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction) :
	origin(origin), direction(direction) {
}

Ray::Ray(float screenX, float screenY, const glm::mat4& proj, const glm::mat4& view) {
	glm::mat4 projInv = glm::inverse(proj);
	glm::mat4 viewInv = glm::inverse(view);

	glm::vec4 rayDirClip = glm::vec4(screenX, screenY, -1.0f, 1.0f);
	glm::vec4 rayDirE = projInv * rayDirClip;
	rayDirE.z = -1.0f; // Forward is down -ve z
	rayDirE.w = 0.0f;  // A direction vector 
	glm::vec4 rayDirW = glm::normalize(viewInv * rayDirE);
	direction = rayDirW;

	glm::vec4 rayStartS = glm::vec4(screenX, screenY, 1.0f, 1.0f);
	glm::vec4 rayStartE = projInv * rayStartS;
	rayStartE.z = 1.0f;
	rayStartE.w = 1.0f;
	glm::vec4 rayStartW = viewInv * rayStartE;
	origin = glm::vec3(rayStartW);
}

glm::vec3 Ray::GetPositionAlong(float offset) const {
	return origin + (offset * direction);
}

Intersection Ray::GetIntersection(const BoundingPlane& plane) const {
	Intersection intersection;
	float normDotDir = glm::dot(plane.normal, direction);
	if (normDotDir == 0) //the ray and plane are parallel
		return intersection;
	float normDotOrigin = glm::dot(plane.normal, origin);
	intersection.offset = -(normDotDir + plane.distanceFromOrigin) / normDotOrigin;
	if (intersection.offset < 0)
		return intersection; // miss, behind view point
	intersection.isIntersecting = true;
	return intersection;
}

std::array<Intersection,6> Ray::GetIntersection(const BoundingBox& bb) const {
	
	glm::vec3 localStart = glm::vec3(bb.invFrame * glm::vec4(origin, 1.0f));
	glm::vec3 localDir = glm::vec3(bb.invFrame * glm::vec4(direction, 0.0f));
	Ray localRay = { localStart, localDir }; //define a new ray relative to the bb
	
	std::array<Intersection, 6> intersections;

	//for each face, check for intersection
	for (int i = bb.FRONT; i <= bb.BOTTOM; i++)
		intersections[i] = localRay.GetIntersection(bb.planes[i]);

	return intersections;
}
