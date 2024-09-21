#pragma once
#include "GraphicsObject.h"
#include "GraphicsStructures.h"
#include <memory>
#include <unordered_set>

class Scene {
private:
	std::unordered_set<std::shared_ptr<GameObject>> objects;
	std::shared_ptr<Light> globalLight;
	std::shared_ptr<Light> localLight;

public:
	inline Scene() : globalLight(nullptr), localLight(nullptr) {}
	inline ~Scene() = default;
	
	inline const std::unordered_set<std::shared_ptr<GameObject>>& GetObjects() const { return objects; }
	inline void AddObject(const std::shared_ptr<GameObject>& object) { objects.insert(object); }

	inline const std::shared_ptr<Light>& GetGlobalLight() const { return globalLight; }
	inline void SetGlobalLight(const std::shared_ptr<Light>& globalLight) { this->globalLight = globalLight; }

	inline const std::shared_ptr<Light>& GetLocalLight() const { return localLight; }
	inline void SetLocalLight(const std::shared_ptr<Light>& localLight) { this->localLight = localLight; }
};
