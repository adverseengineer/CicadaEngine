
#include "Shader.h"
#include "Renderer.h"

Renderer::Renderer(std::shared_ptr<Shader> shader) : shader(shader) {

	glGenVertexArrays(1, &vaoId);
}

void Renderer::AllocateVertexBuffer(const std::vector<std::shared_ptr<GraphicsObject>>& objects) const {

	glBindVertexArray(vaoId); //bind it

	for(auto& obj : objects)
		obj->StaticAllocateVertexBuffer();

	glBindVertexArray(0); //unbind it
}

void Renderer::RenderObject(const GraphicsObject& object) {

	shader->SendMat4Uniform("world", object.GetReferenceFrame());

	auto& buffer = object.GetVertexBuffer();
	buffer->Select();
	buffer->SetUpAttributeInterpretration();
	glDrawArrays(buffer->GetPrimitiveType(), 0, buffer->GetNumberOfVertices());

	// Recursively render the children
	auto& children = object.GetChildren();
	for (auto& child : children)
		RenderObject(*child);
}

void Renderer::RenderScene(const std::shared_ptr<Scene>& scene, const glm::mat4& view) {

	if(shader->IsCreated()) {

		glUseProgram(shader->GetShaderProgram());
		glBindVertexArray(vaoId);

		//we must send the view matrix to the shader every frame
		shader->SendMat4Uniform("view", view);

		//for each object in the scene, render it separately
		for(auto& object : scene->GetObjects())
			RenderObject(*object);

		glDisableVertexAttribArray(0); //position
		glDisableVertexAttribArray(1); //color
		glUseProgram(0); //reset to zero
		glBindVertexArray(0); //unbind it
	}
}
