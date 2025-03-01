#include "Renderer.h"
#include "Shader.h"

using namespace Cicada;

void Renderer::UploadResources(const std::shared_ptr<Scene>& scene) {
	for(const auto& obj : scene->GetObjects())
		obj->UploadResources();
}

void Renderer::RenderObject(const std::shared_ptr<GameObject>& object) {

	auto& material = object->GetMaterial();
	auto& tex = material->GetTexture();
	auto& shader = material->GetShader();

	auto& mesh = object->GetMesh();
	if(!mesh) {
		Logger::Write(LogEntry::Level::Warning, "mesh is null");
		return;
	}

	//send the model matrix to the shader
	shader->SetMat4("world", object->GetGlobalTransform());

	mesh->Bind();

	if (tex != nullptr) {
		shader->SetUInt("tex", tex->GetTextureUnit());
		tex->SelectForRendering();
	}

	auto& material_old = object->GetMaterial_OLD();
	if (material != nullptr) {
		shader->SetFloat("materialAmbientIntensity", material_old->ambientIntensity);
		shader->SetFloat("materialSpecularIntensity", material_old->specularIntensity);
		shader->SetFloat("materialShininess", material_old->shininess);
	}

	glDrawElements(mesh->GetPrimitiveType(), (GLsizei) mesh->IndexElemCount(), GL_UNSIGNED_SHORT, (void*)0);
	
	//recursively render the children
	for (const auto& child : object->GetChildren())
		RenderObject(child);

	mesh->Unbind();
}

//TODO: why does this still take a shader for the whole scene?
void Renderer::RenderScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Camera>& cam) {

	if(shader->GetShaderProg() != 0) {

		//send the data for the global light source
		auto& globalLight = scene->GetGlobalLight();
		if (globalLight != nullptr) {
			shader->SetVec3("globalLightPosition", globalLight->position);
			shader->SetVec3("globalLightColor", globalLight->color);
			shader->SetFloat("globalLightIntensity", globalLight->intensity);
		}
		//send the data for the local light source
		auto& localLight = scene->GetLocalLight();
		if (localLight != nullptr) {
			shader->SetVec3("localLightPosition", localLight->position);
			shader->SetVec3("localLightColor", localLight->color);
			shader->SetFloat("localLightIntensity", localLight->intensity);
			shader->SetFloat("localLightAttenuationCoef", localLight->attenuationCoef);
		}

		//for each object in the scene, render it separately
		for(auto& object : scene->GetObjects())
			RenderObject(object);
	}
	else
		Logger::Write(LogEntry::Level::Warning, "this shader was not linked");
}
