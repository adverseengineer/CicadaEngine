#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Scene>& scene) :
	shader(shader), scene(scene) {
}

void Renderer::StaticAllocateBuffers() const {
	for(const auto& obj : scene->GetObjects())
		obj->StaticAllocate();
}

void Renderer::RenderObject(const std::shared_ptr<GameObject>& object) const {

	//send the model matrix to the shader
	object->GetShader()->SetUniform("world", object->GetGlobalReferenceFrame());

	auto& mesh = object->GetMesh();
	if(mesh == nullptr) {
		Util::Log("mesh is null");
		return;
	}

	mesh->Bind();
	auto& vBuf = mesh->GetVertexBuffer();
	vBuf.Bind();

	auto& shader = object->GetShader();
	auto& tex = object->GetTexture();

	if (tex != nullptr) {
		shader->SetUniform("tex", object->GetTexture()->GetTextureUnit());
		tex->Bind();
	}

	auto& material = object->GetMaterial();
	if (material != nullptr) {
		shader->SetUniform("materialAmbientIntensity", material->ambientIntensity);
		shader->SetUniform("materialSpecularIntensity", material->specularIntensity);
		shader->SetUniform("materialShininess", material->shininess);
	}

	vBuf.SetUpAttributeInterpretration();

	if (mesh->HasIndices()) {
		auto& iBuf = mesh->GetIndexBuffer();
		iBuf.Bind();
		glDrawElements(vBuf.GetPrimitiveType(), (GLsizei) iBuf.GetCount(), GL_UNSIGNED_SHORT, (void*)0);
		iBuf.Unbind();
	}
	else {
		glDrawArrays(vBuf.GetPrimitiveType(), 0, (GLsizei) vBuf.GetNumberOfVertices());
	}

	// Recursively render the children
	auto& children = object->GetChildren();
	for (const auto& child : children)
		RenderObject(child);

	mesh->Unbind();
}

void Renderer::RenderScene(const std::shared_ptr<Camera>& cam) const {

	if(shader->GetShaderProg() != 0) {

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

		//for each object in the scene, render it separately
		for(auto& object : scene->GetObjects())
			RenderObject(object);
	}
	else
		Util::Log("this renderer's shader was not linked");
}
