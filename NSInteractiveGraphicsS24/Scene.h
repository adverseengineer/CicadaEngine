#pragma once
#include "GraphicsObject.h"
#include "GraphicsStructures.h"
#include <memory>
#include <unordered_set>

class Scene {
private:
	std::unordered_set<std::shared_ptr<GraphicsObject>> objects;
	std::shared_ptr<Light> globalLight;
	std::shared_ptr<Light> localLight;

public:
	inline Scene() : globalLight(nullptr), localLight(nullptr) {}
	inline ~Scene() = default;
	
	//gets a const reference to the beginning and end of the object set
	inline const auto GetObjects() const {
		return objects;
	}
	inline void AddObject(const std::shared_ptr<GraphicsObject>& object) {
		objects.insert(object);
	}

	inline const auto& GetGlobalLight() const { return globalLight; }
	inline void SetGlobalLight(const std::shared_ptr<Light>& globalLight) { this->globalLight = globalLight; }

	inline const auto& GetLocalLight() const { return localLight; }
	inline void SetLocalLight(const std::shared_ptr<Light>& localLight) { this->localLight = localLight; }
};
