#pragma once

#include <entt/entt.hpp>

namespace Cicada {

class Renderer {
private:
	Renderer() = default;
	~Renderer() = default;

public:
	static void Render(entt::registry& reg);
};

}
