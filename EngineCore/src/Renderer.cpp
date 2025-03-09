
#include "Renderer.h"
#include "SceneManager.h"
#include "Shader.h"
#include "ecs/components/MeshComponent.h"
#include "ecs/components/MaterialComponent.h"
#include "ecs/components/TransformComponent.h"
#include "GraphicsStructures.h"

using namespace Cicada;
using namespace Cicada::ECS;

void Renderer::Render(entt::registry& reg) {
	
	#pragma region AbysmalDogshit
	//first, send shader uniforms that we might be missing from the old style render function
	auto shader = Shader::Get("diffuse");
	static auto& sm = SceneManager::Instance();
	static auto& localLight = sm.GetLight("local");
	static auto& globalLight = sm.GetLight("global");

	shader->SetVec3("localLightPosition", localLight.position);
	shader->SetVec3("localLightColor", localLight.color);
	shader->SetFloat("localLightIntensity", localLight.intensity);
	shader->SetFloat("localLightAttenuationCoef", localLight.attenuationCoef);
	shader->SetVec3("globalLightPosition", globalLight.position);
	shader->SetVec3("globalLightColor", globalLight.color);
	shader->SetFloat("globalLightIntensity", globalLight.intensity);
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
