#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Scene>& scene) :
	shader(shader), scene(scene), view(1.0f), proj(1.0f) {
	glGenVertexArrays(1, &vaoId);
}

void Renderer::StaticAllocateBuffers(void) const {

	glBindVertexArray(vaoId); //bind it

	for(const auto& obj : scene->GetObjects())
		obj->StaticAllocateBuffers();

	glBindVertexArray(0); //unbind it
}

void Renderer::RenderObject(const std::shared_ptr<GraphicsObject>& object) const {

	//send the model matrix to the shader
	shader->SendMat4Uniform("world", object->GetGlobalReferenceFrame());

	auto& vertBuf = object->GetVertexBuffer();
	if(vertBuf == nullptr) {
		Util::Log("vertex buffer is null");
		return;
	}

	vertBuf->Select();
	
	if (vertBuf->HasTexture()) {
		shader->SendIntUniform("tex", vertBuf->GetTextureUnit());
		vertBuf->GetTexture()->Bind();
	}

	auto& material = object->GetMaterial();
	if (material != nullptr) {
		shader->SendFloatUniform("materialAmbientIntensity", material->ambientIntensity);
		shader->SendFloatUniform("materialSpecularIntensity", material->specularIntensity);
		shader->SendFloatUniform("MaterialShininess", material->shininess);
	}

	vertBuf->SetUpAttributeInterpretration();

	if (object->IsIndexed()) {
		auto& idxBuf = object->GetIndexBuffer();
		idxBuf->Select();
		glDrawElements(vertBuf->GetPrimitiveType(), (GLsizei) idxBuf->GetCount(), GL_UNSIGNED_SHORT, (void*)0);
		idxBuf->Deselect();
	}
	else {
		glDrawArrays(vertBuf->GetPrimitiveType(), 0, (GLsizei) vertBuf->GetNumberOfVertices());
	}

	// Recursively render the children
	auto& children = object->GetChildren();
	for (const auto& child : children)
		RenderObject(child);
}

void Renderer::RenderScene(const std::shared_ptr<Camera>& cam) const {

	if(shader->IsCreated()) {

		glUseProgram(shader->GetShaderProgram());
		glBindVertexArray(vaoId);

		//we must send the view matrix to the shader every frame
		shader->SendMat4Uniform("view", view);
		
		//send the data for the global light source
		auto& globalLight = scene->GetGlobalLight();
		if (globalLight != nullptr) {
			shader->SendVec3Uniform("globalLightPosition", globalLight->position);
			shader->SendVec3Uniform("globalLightColor", globalLight->color);
			shader->SendFloatUniform("globalLightIntensity", globalLight->intensity);
		}
		//send the data for the local light source
		auto& localLight = scene->GetLocalLight();
		if (localLight != nullptr) {
			shader->SendVec3Uniform("localLightPosition", localLight->position);
			shader->SendVec3Uniform("localLightColor", localLight->color);
			shader->SendFloatUniform("localLightIntensity", localLight->intensity);
			shader->SendFloatUniform("localLightAttenuationCoef", localLight->attenuationCoef);
		}

		//send the camera position
		shader->SendVec3Uniform("viewPosition", cam->GetPosition());

		//for each object in the scene, render it separately
		for(auto& object : scene->GetObjects())
			RenderObject(object);

		glDisableVertexAttribArray(0); //position
		glDisableVertexAttribArray(1); //color
		glUseProgram(0); //unbind shader
		glBindVertexArray(0); //unbind vao
	}
	else
		Util::Log("this renderer's shader was not linked");
}
