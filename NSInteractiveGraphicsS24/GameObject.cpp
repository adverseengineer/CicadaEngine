#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

bool GameObject::AddChild(const std::shared_ptr<GameObject>& child) {
	const auto result = children.insert(child);
	if (result.second)
		child->parent = this;
	return result.second;
}

bool GameObject::IsIntersectingRay(const Ray& ray) const {
	return ray.IsIntersectingObject(*this);
}

const glm::vec3 GameObject::GetPosition() const {
	return glm::vec3(referenceFrame[3]);
}

void GameObject::SetPosition(const glm::vec3& position) {
	referenceFrame[3] = glm::vec4(position, 1.0f);
}

void GameObject::ResetOrientation() {
	glm::vec4 position = referenceFrame[3];
	referenceFrame = glm::mat4(1.0f);
	referenceFrame[3] = position;
}

void GameObject::RotateLocalX(float degrees) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void GameObject::RotateLocalY(float degrees) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void GameObject::RotateLocalZ(float degrees) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void GameObject::RotateLocal(float xDeg, float yDeg, float zDeg) {
	referenceFrame = glm::rotate(referenceFrame, glm::radians(xDeg), glm::vec3(1.0f, 0.0f, 0.0f));
	referenceFrame = glm::rotate(referenceFrame, glm::radians(yDeg), glm::vec3(0.0f, 1.0f, 0.0f));
	referenceFrame = glm::rotate(referenceFrame, glm::radians(zDeg), glm::vec3(0.0f, 0.0f, 1.0f));
}

void GameObject::RotateToFace(const glm::vec3& target) {

	glm::vec3 position = referenceFrame[3];
	glm::vec3 zAxis = glm::normalize(target - position);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);
	referenceFrame[0] = glm::vec4(xAxis, 0.0f);
	referenceFrame[1] = glm::vec4(yAxis, 0.0f);
	referenceFrame[2] = glm::vec4(zAxis, 0.0f);
}

void GameObject::UploadResources() const {

	//only concern ourselves with mesh and material if there is a mesh
	if (m_mesh) {

		m_mesh->Setup();
		m_mesh->Upload();
 
		if (m_material) {
			auto& tex = m_material->GetTexture();
			if(tex) tex->Upload();
		}
	}

	for (auto& child : children)
		child->UploadResources();
}

void GameObject::Update(double elapsedSeconds) {
	//TODO: any per-frame updates that an object may need
}
