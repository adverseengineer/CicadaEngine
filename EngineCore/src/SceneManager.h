#pragma once

#include "GameObject.h"
#include "GraphicsStructures.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <entt/entt.hpp>

#include "ecs/components/MeshComponent.h"
#include "ecs/components/MaterialComponent.h"

namespace Cicada {

class SceneManager {
private:
	
	std::unordered_map<std::string, Light> m_lights;

	std::unordered_set<std::shared_ptr<GameObject>> objects; //TODO: keep for now while switching over

	entt::registry mReg;

	SceneManager() = default;
	~SceneManager() = default;

public:

	inline static SceneManager& Instance() {
		static SceneManager instance;
		return instance;
	}

	inline void DGB_SetupTestScene() {
		
		auto& names = mReg.storage<std::string>();

		auto crate = mReg.create();
		names.emplace(crate, "crate");
		//mReg.emplace<Cicada::ECS::Components::MeshComponent>(crate);
		//mReg.emplace<Cicada::ECS::Components::MaterialComponent>(crate);
	}

	inline const std::unordered_set<std::shared_ptr<GameObject>>& GetObjects() const { return objects; }
	inline bool AddObject(const std::shared_ptr<GameObject>& object) { return objects.insert(object).second; }
};

}