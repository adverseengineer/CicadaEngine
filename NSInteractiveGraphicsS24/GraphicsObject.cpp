#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

bool GraphicsObject::AddChild(const std::shared_ptr<GraphicsObject>& child) {
	const auto result = children.insert(child);
	if (result.second)
		child->parent = this;
	return result.second;
}

const glm::vec3& GraphicsObject::GetPosition() const {
	return glm::vec3(referenceFrame[3]);
}

void GraphicsObject::SetPosition(const glm::vec3& position) {
	referenceFrame[3] = glm::vec4(position, 1.0f);
}

void GraphicsObject::ResetOrientation() {
	glm::vec4 position = referenceFrame[3];
	referenceFrame = glm::mat4(1.0f);
	referenceFrame[3] = position;
}

void GraphicsObject::RotateLocalX(float degrees) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void GraphicsObject::RotateLocalY(float degrees) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void GraphicsObject::RotateLocalZ(float degrees) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void GraphicsObject::RotateLocal(float xDeg, float yDeg, float zDeg) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(xDeg), glm::vec3(1.0f, 0.0f, 0.0f));
	referenceFrame = glm::rotate(referenceFrame, glm::radians(yDeg), glm::vec3(0.0f, 1.0f, 0.0f));
	referenceFrame = glm::rotate(referenceFrame, glm::radians(zDeg), glm::vec3(0.0f, 0.0f, 1.0f));
}

void GraphicsObject::RotateToFace(const glm::vec3& target) {

	glm::vec3 position = referenceFrame[3];
	glm::vec3 zAxis = glm::normalize(target - position);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);
	referenceFrame[0] = glm::vec4(xAxis, 0.0f);
	referenceFrame[1] = glm::vec4(yAxis, 0.0f);
	referenceFrame[2] = glm::vec4(zAxis, 0.0f);
}

void GraphicsObject::StaticAllocateBuffers() const {
	vertBuf->Select();
	vertBuf->StaticAllocate();
	vertBuf->Deselect();

	if (idxBuf != nullptr) {
		idxBuf->Select();
		idxBuf->StaticAllocate();
		idxBuf->Deselect();
	}
	
	for (auto& child : children)
		child->StaticAllocateBuffers();
}

void GraphicsObject::Update(double elapsedSeconds) {
	if (animation != nullptr) {
		animation->Update(elapsedSeconds);
	}
	//TODO: any other per-frame updates that an object may need
}
