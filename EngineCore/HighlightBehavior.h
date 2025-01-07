#pragma once
#include "Behavior.h"
#include "GraphicsObject.h"
#include "GraphicsStructures.h"

class HighlightBehavior : public Behavior {
protected:
	HighlightParams params;
	float ambientIntensity = 0.0f;
public:
	inline void StoreDefaults() {
		if (object == nullptr) return;
		auto& material = object->GetMaterial();
		if (material == nullptr) return;
		ambientIntensity = material->ambientIntensity;
	}

	inline void SetParameter(BehaviorParams& params) {
		this->params = reinterpret_cast<HighlightParams&>(params);
	}

	inline void Update(double elapsedSeconds) {
		if (object == nullptr) return;
		auto& material = object->GetMaterial();
		if(material == nullptr) return;
		if ((*params.ray).IsIntersectingObject(*object))
			material->ambientIntensity = 1.0f;
		else
			material->ambientIntensity = ambientIntensity;
	}
};

