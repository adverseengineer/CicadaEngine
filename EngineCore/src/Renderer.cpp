
#include "Renderer.h"
#include "Shader.h"
#include "ecs/components/MeshComponent.h"
#include "ecs/components/MaterialComponent.h"
#include "ecs/components/TransformComponent.h"

using namespace Cicada;
using namespace Cicada::ECS;

void Renderer::RenderObject(const std::shared_ptr<GameObject>& object) {

	auto& material = object->GetMaterial();
	auto& tex = material->GetTexture();
	auto& shader = material->GetShader();

	auto& mesh = object->GetMesh();
	assert(mesh != nullptr);

	//send the model matrix to the shader
	shader->SetMat4("world", object->GetGlobalTransform());

	mesh->Bind();

	assert(tex != nullptr);
	shader->SetUInt("tex", tex->GetTextureUnit());
	tex->SelectForRendering();

	auto& material_old = object->GetMaterial_OLD();
	if (material_old != nullptr) {
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
void Renderer::RenderScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader, const Camera* cam) {
	
	assert(shader->GetShaderProg() != 0);

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

//TODO: these lights shouldn't be params, they should be components in the registry. do this after switching to ECS (will need UBO support)
void Renderer::Render(entt::registry& reg, const std::shared_ptr<Light>& globalLight, const std::shared_ptr<Light>& localLight) {
	
	#pragma region AbysmalDogshit
	//first, send shader uniforms that we might be missing from the old style render function
	assert(globalLight != nullptr);
	assert(localLight != nullptr);
	auto shader = Shader::Get("diffuse");

	shader->SetVec3("globalLightPosition", globalLight->position);
	shader->SetVec3("globalLightColor", globalLight->color);
	shader->SetFloat("globalLightIntensity", globalLight->intensity);
	shader->SetVec3("localLightPosition", localLight->position);
	shader->SetVec3("localLightColor", localLight->color);
	shader->SetFloat("localLightIntensity", localLight->intensity);
	shader->SetFloat("localLightAttenuationCoef", localLight->attenuationCoef);
	#pragma endregion

	std::vector<std::tuple<glm::mat4, std::shared_ptr<Mesh>, std::shared_ptr<Material>>> renderQueue;

	auto view = reg.view<TransformComponent, MaterialComponent, MeshComponent>();
	for (auto entity : view) {
		auto& transform = view.get<TransformComponent>(entity);
		auto& mesh = view.get<MeshComponent>(entity);
		auto& material = view.get<MaterialComponent>(entity);

		renderQueue.emplace_back(transform.localTransform, mesh.ptr, material.ptr);
	}

	//TODO: figure out the best metric to sort the renderQueue by
	//std::sort(renderQueue.begin(), renderQueue.end(), [](auto& a, auto& b) {
	//	return std::get<0>(a) < std::get<0>(b);
	//});

	//actually render all the enqueued data
	for (auto& [transform, mesh, material] : renderQueue) {

		shader->SetMat4("world", transform);
		mesh->Bind();

		shader->SetUInt("tex", material->GetTexture()->GetTextureUnit());
		material->Bind();
		glDrawElements(mesh->GetPrimitiveType(), (GLsizei)mesh->IndexElemCount(), GL_UNSIGNED_SHORT, (void*)0);
	}
}
