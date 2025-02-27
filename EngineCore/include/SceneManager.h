#pragma once

#include "GameObject.h"
#include "GraphicsStructures.h"
#include <memory>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include <entt/entt.hpp>

namespace Cicada {

using EntityID = uint64_t;

class SceneManager {
private:
	
	std::unordered_map<std::string, Light> m_lights;

	std::unordered_set<std::shared_ptr<GameObject>> objects; //TODO: keep for now while switching over

	SceneManager() = default;
	~SceneManager() = default;

	//TODO: come back to this and upgrade it if performance is bad. use this:
	//https://skypjack.github.io/2019-08-20-ecs-baf-part-4-insights/
public:

	inline static SceneManager& Instance() {
		static SceneManager instance;
		return instance;
	}

	inline std::optional<EntityID> CreateEntity(EntityID parent = 0);
	inline std::optional<EntityID> LoadEntityGraph(EntityID parent = 0);

	inline const std::unordered_set<std::shared_ptr<GameObject>>& GetObjects() const { return objects; }
	inline bool AddObject(const std::shared_ptr<GameObject>& object) { return objects.insert(object).second; }
};

}