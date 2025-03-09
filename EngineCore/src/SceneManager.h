#pragma once

#include "GraphicsStructures.h"
#include <entt/entt.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>

//#include "ecs/components/MeshComponent.h"
//#include "ecs/components/MaterialComponent.h"

namespace Cicada {

class SceneManager {
private:
	
	std::unordered_map<std::string, Light> m_lights;

	entt::registry m_reg;

	SceneManager() = default;
	~SceneManager() = default;

public:

	inline static SceneManager& Instance() {
		static SceneManager instance;
		return instance;
	}

	inline Light& GetLight(std::string_view name) {
		assert(m_lights.contains(name.data())); //TODO: fine for now, ill use lightcomponents soon
		return m_lights.at(name.data());
	}

	inline void SetLight(std::string_view name, const Light& light) {
		assert(!m_lights.contains(name.data()));
		m_lights.emplace(name.data(), light);
	}
};

}