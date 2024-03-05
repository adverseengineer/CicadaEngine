#include "GraphicsObject.h"
#include <glm/gtc/matrix_transform.hpp>

GraphicsObject::GraphicsObject() : referenceFrame(1.0f), parent(nullptr) {}

glm::mat4 GraphicsObject::GetReferenceFrame(void) const {

	if (parent != nullptr)
		return referenceFrame * parent->referenceFrame;

	return referenceFrame;
}

void GraphicsObject::CreateVertexBuffer(unsigned int numberOfElementsPerVertex) {
	vBuf = std::make_shared<VertexBuffer>(numberOfElementsPerVertex);
}

void GraphicsObject::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuf) {
	this->vBuf = vBuf;
}

void GraphicsObject::StaticAllocateVertexBuffer(void) {
	vBuf->Select();
	vBuf->StaticAllocate();
	vBuf->Deselect();
	for (auto& child : children)
		child->StaticAllocateVertexBuffer();
}

void GraphicsObject::AddChild(const std::shared_ptr<GraphicsObject>& child) {
	children.push_back(child);
	child->parent = this;
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

void GraphicsObject::Update(double elapsedSeconds) {
	if (animation != nullptr) {
		animation->Update(elapsedSeconds);
	}
}
