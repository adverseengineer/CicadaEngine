#include "Ray.h"
#include "GameObject.h"
#include "BoundingBox.h"

Intersection Ray::GetIntersection(const GeometricPlane& plane) const
{
	GeometricLine line;
	line.SetDirection(direction);
	line.SetStartPoint(origin);
	return plane.GetIntersectionWithLine(line);
}

Intersection Ray::GetIntersection(const BoundingBox& boundingBox) const {
	return Intersection();
}

bool Ray::IsIntersectingObject(const GameObject& object) const {
	if (!object.HasBoundingBox()) return false;
	return object.GetBoundingBox()->IsIntersectingWithRay(*this);
}

