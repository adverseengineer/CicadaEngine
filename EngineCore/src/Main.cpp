#define NOMINMAX //this is needed so that rapidjson doesn't shit the bed, idk why

#include "EventManager.h"
#include "FileSystem.h"
#include "GraphicsContext.h"
#include "GraphicsEnvironment.h"
#include "JsonUtils.h"
#include "ManagedObject.h"
#include "ScriptManager.h"
#include "ui/UISystem.h"

#include "ecs/components/TransformComponent.h"
#include "ecs/components/MaterialComponent.h"
#include "ecs/components/MeshComponent.h"

#include <Windows.h>

#ifdef CreateWindow //fuck you microsoft, i want this function name
#undef CreateWindow
#endif

using namespace Cicada;
using namespace Cicada::ECS;

inline void SetupRegistry(entt::registry& reg) {

	auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");

	auto crateMesh = Mesh::Create("crateMesh", "temp/cube.obj");
	auto ballMesh = Mesh::Create("ballMesh", "temp/uvsphere.obj");
	auto floorMesh = Mesh::Create("floorMesh", "temp/plane.obj");

	auto crateTex = Texture2D::Create("crateTex", "crate.png");
	auto ballTex = Texture2D::Create("ballTex", "not-a-real-texture.png");
	auto floorTex = Texture2D::Create("floorTex", "floor.png");

	auto crateMat = Material::Create("crateMat", diffuseShader, crateTex);
	auto ballMat = Material::Create("ballMat", diffuseShader, ballTex);
	auto floorMat = Material::Create("floorMat", diffuseShader, floorTex);

	auto& names = reg.storage<std::string>();

	auto crate = reg.create();
	names.emplace(crate, "crate");
	reg.emplace<TransformComponent>(crate, glm::vec3{ 10.0, 0.0, 10.0 });
	reg.emplace<MeshComponent>(crate, crateMesh);
	reg.emplace<MaterialComponent>(crate, crateMat);

	auto ball = reg.create();
	names.emplace(ball, "ball");
	reg.emplace<TransformComponent>(ball, glm::vec3{ -10.0, 0.0, 10.0 });
	reg.emplace<MeshComponent>(ball, ballMesh);
	reg.emplace<MaterialComponent>(ball, ballMat);

	auto floor = reg.create();
	names.emplace(floor, "floor");
	reg.emplace<TransformComponent>(floor, glm::vec3{ 0.0, 0.0, 0.0 });
	reg.emplace<MeshComponent>(floor, floorMesh);
	reg.emplace<MaterialComponent>(floor, floorMat);

	auto sunMesh = Mesh::Create("sunMesh", "temp/plane.obj");
	auto sunTex = Texture2D::Create("sunTex", "gw.png");
	auto sunMat = Material::Create("sunMat", diffuseShader, sunTex);

	auto sun = reg.create();
	names.emplace(sun, "sun");
	reg.emplace<TransformComponent>(sun, glm::vec3{ 0.0, 20.0, 0.0 });
	reg.emplace<MeshComponent>(sun, sunMesh);
	reg.emplace<MaterialComponent>(sun, sunMat);
}

template<typename... Components>
void inspect_entity(entt::registry& registry, entt::entity entity) {
	Logger::Writef(LogEntry::Level::Info, "{}", static_cast<uint32_t>(entity));

	//expand pack to check which known components are attached
	([&] {
		if (registry.any_of<Components>(entity)) {
			const auto& component = registry.get<Components>(entity);
			std::cout << "  Component: " << typeid(Components).name() << '\n';
			Logger::Writef(LogEntry::Level::Info, "{}", typeid(Components).name());

			if constexpr (std::is_same_v<Components, TransformComponent>) {
				Logger::Writef(LogEntry::Level::Info, "Position: ({}, {}, {})", component.localTransform[3][0], component.localTransform[3][1], component.localTransform[3][2]);
			}
			else if constexpr (std::is_same_v<Components, MeshComponent>) {
				Logger::Writef(LogEntry::Level::Info, "Mesh: ()");
			}
			else if constexpr (std::is_same_v<Components, MaterialComponent>) {
				Shader* tempShad = component.ptr->GetShader().get();
				Texture* tempTex = component.ptr->GetTexture().get();
				Logger::Writef(LogEntry::Level::Info, "Material: (Shader: {}, Texture: {})", fmt::ptr(tempShad), fmt::ptr(tempTex));
			}
		}
	}(), ...); //expands over all component types
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	//ScriptManager::Init();
	//ScriptManager::LoadScript("test.lua");

	//EventManager::TriggerEvent("OnStart");

	auto& gc = GraphicsContext::Instance();
	gc.CreateWindow(1200, 800, "Cicada Engine");
	gc.InitGlad();
	gc.Configure();

	auto& ge = GraphicsEnvironment::Instance();

	//need to initialize UI after window is made
	auto& ui = UI::UISystem::Instance();

	auto& sm = SceneManager::Instance();

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 500.0f, 1200.0f / 800.0f);
	cam->SetMainCam();
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });

	Light local = {
		glm::vec3{ 0, 10.0f, 0 },
		glm::vec3{ 0.3f, 0.9f, 0.4f },
		1.0, 0.0
	};
	sm.SetLight("local", local);

	Light global = {
		glm::vec3{ 40.0f, 40.0f, 40.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		1.0, 0.5
	};
	sm.SetLight("global", global);
	
	entt::registry reg;
	SetupRegistry(reg);

	//rapidjson::Document sceneSchema, sceneJson;
	//JsonUtils::ReadJson("scene_schema.json", sceneSchema);
	//JsonUtils::ReadJson("test_scene.json", sceneJson);

	//bool ye = JsonUtils::ValidateAgainstSchema(sceneSchema, sceneJson);

	//if (ye)
		//Logger::Write(LogEntry::Level::Info, "All clear!");

	//auto& objectsJson = sceneJson["gameObjects"];

	auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");
	
	for (auto entity : reg.storage<entt::entity>()) {
		inspect_entity<TransformComponent, MeshComponent, MaterialComponent>(reg, entity);
	}

	ge.Run3D(reg);

	return 0;
}
