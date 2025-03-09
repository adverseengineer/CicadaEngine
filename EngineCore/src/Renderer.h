#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Util.h"
#include <sstream>
#include <vector>

#include <entt/entt.hpp>

namespace Cicada {

class Renderer {
private:
	static void RenderObject(const std::shared_ptr<GameObject>&);

	Renderer() = default;
	~Renderer() = default;

public:
	static void RenderScene(const std::shared_ptr<Scene>&, const std::shared_ptr<Shader>&, const Camera*);

	static void Render(entt::registry& reg, const std::shared_ptr<Light>& globalLight, const std::shared_ptr<Light>& localLight);
};

}
