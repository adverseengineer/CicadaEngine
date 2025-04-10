#include "BoundingBox.h"
#include "Ray.h"

using namespace Cicada;

BoundingBox::BoundingBox() : referenceFrame(1.0f) {
	Create(1.0f, 1.0f, 1.0f);
}

void BoundingBox::Create(float width, float height, float depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	float hw = this->width / 2;
	float hh = this->height / 2;
	float hd = this->depth / 2;
	glm::vec3 xAxis = referenceFrame[0];
	glm::vec3 yAxis = referenceFrame[1];
	glm::vec3 zAxis = referenceFrame[2];
	planes[FRONT].Set(-zAxis, hd);
	planes[BACK].Set(zAxis, hd);
	planes[RIGHT].Set(-xAxis, hw);
	planes[LEFT].Set(xAxis, hw);
	planes[TOP].Set(-yAxis, hh);
	planes[BOTTOM].Set(yAxis, hh);
}

bool BoundingBox::IsIntersectingWithRay(const Ray& ray) {
	intersections.clear();
	Intersection intersection;
	Ray localRay;
	glm::vec3 localStart = glm::vec3(inverseMat * glm::vec4(ray.origin, 1.0f));
	localRay.origin = localStart;
	glm::vec3 localDir = glm::vec3(inverseMat * glm::vec4(ray.direction, 0.0f));
	localRay.direction = localDir;
	for (std::size_t i = FRONT; i <= BOTTOM; i++) {
		intersection = localRay.GetIntersection(planes[i]);
		intersections.push_back(intersection);
	}

	// Special case - check if the ray is pointing at the origin
	// of the object
	//if (localRay.IsPointAlongRay({ 0.0f, 0.0f, 0.0f })) return true;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	float nearestFarI = intersections[BoundingBox::BACK].offset;
	float farthestNearI = intersections[BoundingBox::FRONT].offset;
	if (nearestFarI < farthestNearI) {
		std::swap(nearestFarI, farthestNearI);
	}
	float nearI = intersections[BoundingBox::LEFT].offset;
	float farI = intersections[BoundingBox::RIGHT].offset;
	if (nearI > farI) {
		std::swap(nearI, farI);
	}
	if (nearI != farI) {
		if (farI < nearestFarI) nearestFarI = farI;
		if (nearI > farthestNearI) farthestNearI = nearI;
		if (nearestFarI < farthestNearI) return false;
	}

	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	nearI = intersections[BoundingBox::FRONT].offset;
	farI = intersections[BoundingBox::BACK].offset;
	if (nearI > farI) {
		std::swap(nearI, farI);
	}
	if (nearI != farI) {
		if (farI < nearestFarI) nearestFarI = farI;
		if (nearI > farthestNearI) farthestNearI = nearI;
		if (nearestFarI < farthestNearI) return false;
	}

	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	nearI = intersections[BoundingBox::TOP].offset;
	farI = intersections[BoundingBox::BOTTOM].offset;
	if (nearI > farI) {
		std::swap(nearI, farI);
	}
	if (nearI != farI) {
		if (farI < nearestFarI) nearestFarI = farI;
		if (nearI > farthestNearI) farthestNearI = nearI;
		if (nearestFarI < farthestNearI) return false;
	}

	intersectionPoint = ray.Project(farthestNearI);
	return true;
}
