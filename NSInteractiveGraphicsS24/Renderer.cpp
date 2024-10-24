#include "Renderer.h"
#include "Shader.h"

void Renderer::StaticAllocateBuffers(const std::shared_ptr<Scene>& scene) {
	for(const auto& obj : scene->GetObjects())
		obj->StaticAllocate();
}

void Renderer::RenderObject(const std::shared_ptr<GameObject>& object) {

	//send the model matrix to the shader
	object->GetShader()->SendUniform("world", object->GetGlobalReferenceFrame());

	auto& mesh = object->GetMesh();
	if(mesh == nullptr) {
		Util::Log("mesh is null");
		return;
	}

	mesh->Bind();
	//mesh->BindVBO();

	auto& shader = object->GetShader();
	auto& tex = object->GetTexture();

	if (tex != nullptr) {
		shader->SendUniform("tex", object->GetTexture()->GetTextureUnit());
		tex->SelectForRendering();
	}

	auto& material = object->GetMaterial();
	if (material != nullptr) {
		shader->SendUniform("materialAmbientIntensity", material->ambientIntensity);
		shader->SendUniform("materialSpecularIntensity", material->specularIntensity);
		shader->SendUniform("materialShininess", material->shininess);
	}

	//mesh->SetUpAttributeInterpretration();

	//if (mesh->IndexCount() != 0) {
		//mesh->BindIBO();
		glDrawElements(mesh->GetPrimitiveType(), (GLsizei) mesh->IndexElemCount(), GL_UNSIGNED_SHORT, (void*)0);
	//}
	//else {
		//glDrawArrays(mesh->GetPrimitiveType(), 0, (GLsizei) mesh->GetNumberOfVertices());
	//}

	// Recursively render the children
	auto& children = object->GetChildren();
	for (const auto& child : children)
		RenderObject(child);

	mesh->Unbind();
}

void Renderer::RenderScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Camera>& cam) {

	if(shader->GetShaderProg() != 0) {

		//send the data for the global light source
		auto& globalLight = scene->GetGlobalLight();
		if (globalLight != nullptr) {
			shader->SendUniform("globalLightPosition", globalLight->position);
			shader->SendUniform("globalLightColor", globalLight->color);
			shader->SendUniform("globalLightIntensity", globalLight->intensity);
		}
		//send the data for the local light source
		auto& localLight = scene->GetLocalLight();
		if (localLight != nullptr) {
			shader->SendUniform("localLightPosition", localLight->position);
			shader->SendUniform("localLightColor", localLight->color);
			shader->SendUniform("localLightIntensity", localLight->intensity);
			shader->SendUniform("localLightAttenuationCoef", localLight->attenuationCoef);
		}

		//for each object in the scene, render it separately
		for(auto& object : scene->GetObjects())
			RenderObject(object);
	}
	else
		Util::Log("this renderer's shader was not linked");
}
