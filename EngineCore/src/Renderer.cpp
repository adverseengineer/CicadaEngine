
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
	if(!mesh) {
		Logger::Write(LogEntry::Level::Warning, "mesh is null");
		return;
	}

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
void Renderer::RenderScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Camera>& cam) {
	
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

void Renderer::Render(entt::registry& reg) {
	
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
		material->Bind();
		glDrawElements(mesh->GetPrimitiveType(), (GLsizei)mesh->IndexElemCount(), GL_UNSIGNED_SHORT, (void*)0);
	}
}
