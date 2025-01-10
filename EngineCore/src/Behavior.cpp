#include "Behavior.h"

bool Behavior::clicked = false;

void Behavior::SetClickState(bool clicked) {
	Behavior::clicked = clicked;
}

void TranslateAnimation::StoreDefaults() {
	if (object == nullptr) return;
	//TODO: whatever this should do
}
void TranslateAnimation::SetParameter(BehaviorParams& params) {
	this->params = reinterpret_cast<TranslationParams&>(params);
}
void TranslateAnimation::Update(double elapsedSeconds) {
	if (object == nullptr) return;
	//if we are moused over it and holding right click, set the go flag to true
	if (params.ray->IsIntersectingObject(*object) && clicked && !go) go = true;
	//if the go flag isn't on, restart
	if (!go) return;

	float deltaSpeed = static_cast<float>(params.speed * elapsedSeconds);	
	params.progress += deltaSpeed * params.direction;
	//if we've hit either end, stop
	if ((params.progress <= 0.0f) || (params.progress >= 1.0f)) {
		go = false;
		params.direction *= -1;
	}

	glm::vec3 newPos = params.pointA + params.progress * (params.pointB - params.pointA);
	glm::mat4 referenceFrame = object->GetLocalTransform();
	referenceFrame[3] = glm::vec4(newPos, 1.0f);
	object->SetLocalTransform(referenceFrame);
	if (object->HasBoundingBox()) {
		object->GetBoundingBox()->SetReferenceFrame(referenceFrame);
	}
}


void RotateAnimation::SetParameter(BehaviorParams& params) {
	this->params = reinterpret_cast<RotationParams&>(params);
}
void RotateAnimation::StoreDefaults() {
	if (object == nullptr) return;
	//TODO: whatever this should do
	params.rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	params.angularVelocity = 90.0f;
}
void RotateAnimation::Update(double elapsedSeconds) {
	if (object == nullptr) return;
	float mult = (params.ray->IsIntersectingObject(*object)) ? 1.0f : 0.0f;
	float deltaSpeed = static_cast<float>(params.angularVelocity * elapsedSeconds * mult);
	glm::mat4 referenceFrame = object->GetLocalTransform();
	referenceFrame = glm::rotate(referenceFrame, glm::radians(deltaSpeed), params.rotationAxis);
	object->SetLocalTransform(referenceFrame);
	if (object->HasBoundingBox()) {
		object->GetBoundingBox()->SetReferenceFrame(referenceFrame);
	}
}


void HighlightBehavior::SetParameter(BehaviorParams& params) {
	this->params = reinterpret_cast<HighlightParams&>(params);
}
void HighlightBehavior::StoreDefaults() {
	if (object == nullptr) return;
	auto& material = object->GetMaterial_OLD();
	if (material == nullptr) return;
	ambientIntensity = material->ambientIntensity;
}
void HighlightBehavior::Update(double elapsedSeconds) {
	if (object == nullptr) return;
	auto& material = object->GetMaterial_OLD();
	if (material == nullptr) return;
	if ((*params.ray).IsIntersectingObject(*object))
		material->ambientIntensity = 1.0f;
	else
		material->ambientIntensity = ambientIntensity;
}