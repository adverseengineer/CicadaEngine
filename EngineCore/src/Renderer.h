#pragma once

#include <entt/entt.hpp>

namespace Cicada {

class Renderer {
private:
	Renderer() = default;
	~Renderer() = default;

	static bool s_wireframeEnabled;

public:
	static void Render(entt::registry& reg);
	static void ToggleWireframe();
};

}
