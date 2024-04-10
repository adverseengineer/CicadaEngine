#pragma once
#include "Camera.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Util.h"
#include <sstream>
#include <vector>

class Renderer {
private:
	unsigned int vaoId;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Scene> scene;
	glm::mat4 view;
	glm::mat4 proj;

	void RenderObject(const std::shared_ptr<GraphicsObject>& object) const;

public:
	Renderer(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Scene>& scene);
	inline ~Renderer() = default;

	inline const std::shared_ptr<Shader>& GetShader() const { return shader; }
	inline void SetShader(const std::shared_ptr<Shader>& shader) { this->shader = shader; }

	inline const std::shared_ptr<Scene>& GetScene() const { return scene; }
	inline void GetScene(const std::shared_ptr<Scene>& scene) { this->scene = scene; }

	inline const glm::mat4& GetView() { return view; }
	inline void SetView(const glm::mat4& view) { this->view = view; }

	inline const glm::mat4& GetProjection() { return proj; }
	inline void SetProjection(const glm::mat4& proj) { this->proj = proj; }
	
	void StaticAllocateBuffers() const;

	void RenderScene(const std::shared_ptr<Camera>& cam) const;
};
