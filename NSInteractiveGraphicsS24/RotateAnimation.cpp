#include "GraphicsObject.h"
#include "RotateAnimation.h"
#include <glm/gtc/matrix_transform.hpp>

void RotateAnimation::Update(double elapsedSeconds) {

	if (object == nullptr)
		return;

	float deltaSpeed = static_cast<float>(speed * elapsedSeconds);
	glm::mat4 referenceFrame = object->GetReferenceFrame();
	referenceFrame = glm::rotate(referenceFrame, glm::radians(deltaSpeed), axis);
	object->SetReferenceFrame(referenceFrame);
}
