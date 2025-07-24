
#include "ecs/components/MaterialComponent.h"
#include "ecs/components/MeshComponent.h"
#include "ecs/components/TransformComponent.h"
#include "GraphicsStructures.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Shader.h"

using namespace Cicada;
using namespace Cicada::ECS;

bool Renderer::s_wireframeEnabled = false;

void Renderer::Render(entt::registry& reg) {
	
	static auto& sm = SceneManager::Instance();
	static auto& localLight = sm.GetLight("local");
	static auto& globalLight = sm.GetLight("global");
	
	Shader::ForEach([&](auto shader) {
		//shader->SetVec3("localLightPosition", localLight.position);
		//shader->SetVec3("localLightColor", localLight.color);
		//shader->SetFloat("localLightIntensity", localLight.intensity);
		//shader->SetFloat("localLightAttenuationCoef", localLight.attenuationCoef);
		//shader->SetVec3("globalLightPosition", globalLight.position);
		//shader->SetVec3("globalLightColor", globalLight.color);
		//shader->SetFloat("globalLightIntensity", globalLight.intensity);
	});
	
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

		auto& shader = material->GetShader();
		auto& texture = material->GetTexture();

		if (shader == nullptr)
			continue;

		shader->SetMat4("world", transform);
		mesh->Bind();

		if (texture != nullptr)
			shader->SetUInt("tex", texture->GetTextureUnit());
	
		material->Bind();
		glDrawElements(mesh->GetPrimitiveType(), (GLsizei)mesh->IndexElemCount(), GL_UNSIGNED_SHORT, (void*)0);
	}
}

void Renderer::ToggleWireframe() {
	s_wireframeEnabled = !s_wireframeEnabled;
	if (s_wireframeEnabled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
