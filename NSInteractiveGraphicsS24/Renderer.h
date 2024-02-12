#pragma once
#include <sstream>
#include <vector>

#include "BaseObject.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include "Shader.h"

class Renderer : BaseObject {
private:
	unsigned int vaoId;
	std::shared_ptr<Shader> shader;

	void RenderObject(const GraphicsObject& object);

public:
	Renderer(std::shared_ptr<Shader> shader);
	~Renderer(void) = default;

	inline std::shared_ptr<Shader> GetShader(void) const {
		return shader;
	}

	void AllocateVertexBuffer(const std::vector<std::shared_ptr<GraphicsObject>>& objects) const;
	void RenderScene(const std::shared_ptr<Scene>& Scene, const glm::mat4& view);
};
