#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

bool GraphicsObject::AddChild(const std::shared_ptr<GraphicsObject>& child) {
	const auto result = children.insert(child);
	if (result.second)
		child->parent = this;
	return result.second;
}

const auto& GraphicsObject::GetPosition() const {
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

void GraphicsObject::StaticAllocateVertexBuffer() const {
	vertBuf->Select();
	vertBuf->StaticAllocate();
	vertBuf->Deselect();
	for (auto& child : children)
		child->StaticAllocateVertexBuffer();
}

void GraphicsObject::Update(double elapsedSeconds) {
	if (animation != nullptr) {
		animation->Update(elapsedSeconds);
	}
	//TODO: any other per-frame updates that an object may need
}
