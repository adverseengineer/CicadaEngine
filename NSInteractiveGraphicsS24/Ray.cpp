#include "Ray.h"
#include "GraphicsObject.h"
#include "BoundingBox.h"

Intersection Ray::GetIntersectionWithPlane(const GeometricPlane& plane) const
{
	GeometricLine line;
	line.SetDirection(direction);
	line.SetStartPoint(origin);
	return plane.GetIntersectionWithLine(line);
}

Intersection Ray::GetIntersectionWithBoundingBox(const BoundingBox& boundingBox) const {
	return Intersection();
}

bool Ray::IsIntersectingObject(const GameObject& object) const {
	if (!object.HasBoundingBox()) return false;
	return object.GetBoundingBox()->IsIntersectingWithRay(*this);
}

//bool Ray::IsIntersectingObject(const std::shared_ptr<GraphicsObject>& object) const {
	//if(object == nullptr) return false;
	//return IsIntersectingObject(*object);
//}
