#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Util.h"
#include <sstream>
#include <vector>

class Renderer {
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Scene> scene;

	void RenderObject(const std::shared_ptr<GameObject>& object) const;

public:
	Renderer(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Scene>& scene);
	inline ~Renderer() = default;

	inline const std::shared_ptr<Shader>& GetShader() const { return shader; }
	inline void SetShader(const std::shared_ptr<Shader>& shader) { this->shader = shader; }

	inline const std::shared_ptr<Scene>& GetScene() const { return scene; }
	inline void GetScene(const std::shared_ptr<Scene>& scene) { this->scene = scene; }

	void StaticAllocateBuffers() const;

	void RenderScene(const std::shared_ptr<Camera>& cam) const;
};
