#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Scene>& scene) :
	shader(shader), scene(scene), view(1.0f), proj(1.0f) {
	glGenVertexArrays(1, &vaoId);
}

void Renderer::StaticAllocateBuffers(void) const {

	glBindVertexArray(vaoId); //bind it

	for(const auto& obj : scene->GetObjects())
		obj->StaticAllocate();

	glBindVertexArray(0); //unbind it
}

void Renderer::RenderObject(const std::shared_ptr<GameObject>& object) const {

	//send the model matrix to the shader
	object->GetShader()->SetUniform("world", object->GetGlobalReferenceFrame());

	auto& vertBuf = object->GetVertexBuffer();
	if(vertBuf == nullptr) {
		Util::Log("vertex buffer is null");
		return;
	}

	vertBuf->Bind();

	if (object->HasTexture()) {
		object->GetShader()->SetUniform("tex", object->GetTexture()->GetTextureUnit());
		object->GetTexture()->Bind();
	}

	auto& material = object->GetMaterial();
	if (material != nullptr) {
		object->GetShader()->SetUniform("materialAmbientIntensity", material->ambientIntensity);
		object->GetShader()->SetUniform("materialSpecularIntensity", material->specularIntensity);
		object->GetShader()->SetUniform("materialShininess", material->shininess);
	}

	vertBuf->SetUpAttributeInterpretration();

	if (object->IsIndexed()) {
		auto& idxBuf = object->GetIndexBuffer();
		idxBuf->Bind();
		glDrawElements(vertBuf->GetPrimitiveType(), (GLsizei) idxBuf->GetCount(), GL_UNSIGNED_SHORT, (void*)0);
		idxBuf->Unbind();
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

	if(shader->GetShaderProg() != 0) {

		glUseProgram(shader->GetShaderProg());
		glBindVertexArray(vaoId);

		//we must send the view matrix to the shader every frame
		shader->SetUniform("view", view);
		
		//send the data for the global light source
		auto& globalLight = scene->GetGlobalLight();
		if (globalLight != nullptr) {
			shader->SetUniform("globalLightPosition", globalLight->position);
			shader->SetUniform("globalLightColor", globalLight->color);
			shader->SetUniform("globalLightIntensity", globalLight->intensity);
		}
		//send the data for the local light source
		auto& localLight = scene->GetLocalLight();
		if (localLight != nullptr) {
			shader->SetUniform("localLightPosition", localLight->position);
			shader->SetUniform("localLightColor", localLight->color);
			shader->SetUniform("localLightIntensity", localLight->intensity);
			shader->SetUniform("localLightAttenuationCoef", localLight->attenuationCoef);
		}

		//send the camera position
		shader->SetUniform("viewPosition", cam->GetPosition());

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
