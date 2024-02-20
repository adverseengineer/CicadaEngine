
#include "Shader.h"
#include "Renderer.h"

Renderer::Renderer(const std::shared_ptr<Shader>& shadPtr, const std::shared_ptr<Scene>& scenePtr) : shadPtr(shadPtr), scenePtr(scenePtr) {

	glGenVertexArrays(1, &vaoId);
	view = glm::mat4();
}

void Renderer::AllocateVertexBuffer(const std::vector<std::shared_ptr<GraphicsObject>>& objects) const {

	glBindVertexArray(vaoId); //bind it

	for(auto& obj : objects)
		obj->StaticAllocateVertexBuffer();

	glBindVertexArray(0); //unbind it
}

void Renderer::RenderObject(const GraphicsObject& object) {

	//send the model matrix to the shader
	shadPtr->SendMat4Uniform("world", object.GetReferenceFrame());

	auto& buffer = object.GetVertexBuffer();
	buffer->Select();
	
	if (buffer->HasTexture()) {
		shadPtr->SendIntUniform("texUnit", buffer->GetTextureUnit());
		buffer->GetTexturePtr()->SelectToRender();
	}

	buffer->SetUpAttributeInterpretration();
	glDrawArrays(buffer->GetPrimitiveType(), 0, buffer->GetNumberOfVertices());

	// Recursively render the children
	auto& children = object.GetChildren();
	for (auto& child : children)
		RenderObject(*child);
}

void Renderer::RenderScene(void) {

	if(shadPtr->IsCreated()) {

		glUseProgram(shadPtr->GetShaderProgram());
		glBindVertexArray(vaoId);

		//we must send the view matrix to the shader every frame
		shadPtr->SendMat4Uniform("view", view);

		//for each object in the scene, render it separately
		for(auto& object : scenePtr->GetObjects())
			RenderObject(*object);

		glDisableVertexAttribArray(0); //position
		glDisableVertexAttribArray(1); //color
		glUseProgram(0); //reset to zero
		glBindVertexArray(0); //unbind it
	}
}
