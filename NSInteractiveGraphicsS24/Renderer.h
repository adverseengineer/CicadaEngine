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
	std::shared_ptr<Shader> shadPtr;
	std::shared_ptr<Scene> scenePtr;
	glm::mat4 view;

	void RenderObject(const GraphicsObject& object);

public:
	Renderer(const std::shared_ptr<Shader>& shadPtr, const std::shared_ptr<Scene>& scenePtr);
	~Renderer(void) = default;

	inline std::shared_ptr<Shader> GetShader(void) const { return shadPtr; }
	inline void SetShader(const std::shared_ptr<Shader>&) { this->shadPtr = shadPtr; }
	inline std::shared_ptr<Scene> GetScene(void) const { return scenePtr; }
	inline void SetScene(const std::shared_ptr<Scene>&) { this->scenePtr = scenePtr; }
	inline glm::mat4 GetView(void) const { return view; }
	inline void SetView(const glm::mat4 & view) { this->view = view; }

	void AllocateVertexBuffer(const std::vector<std::shared_ptr<GraphicsObject>>& objects) const;
	void RenderScene(void);
};
