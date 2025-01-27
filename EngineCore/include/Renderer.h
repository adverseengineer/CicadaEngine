#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Util.h"
#include <sstream>
#include <vector>

namespace Cicada {

class Renderer {
private:
	static void RenderObject(const std::shared_ptr<GameObject>& object);

public:
	inline Renderer() = delete;

	static void UploadResources(const std::shared_ptr<Scene>& scene);
	static void RenderScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Camera>& cam);
};

}
