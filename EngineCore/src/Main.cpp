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
	auto ballMesh = Mesh::Create("ballMesh", "temp/ball.obj");
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
	reg.emplace<TransformComponent>(crate, glm::mat4{1.0});
	reg.emplace<MeshComponent>(crate, crateMesh);
	reg.emplace<MaterialComponent>(crate, crateMat);

	auto ball = reg.create();
	names.emplace(ball, "ball");
	reg.emplace<TransformComponent>(ball, glm::mat4{1.0});
	reg.emplace<MeshComponent>(ball, ballMesh);
	reg.emplace<MaterialComponent>(ball, ballMat);

	auto floor = reg.create();
	names.emplace(floor, "floor");
	reg.emplace<TransformComponent>(floor, glm::mat4{1.0});
	reg.emplace<MeshComponent>(floor, floorMesh);
	reg.emplace<MaterialComponent>(floor, floorMat);
}

static void SetUp3DScene(std::shared_ptr<Scene>& scene) {

	//auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");
	//diffuseShader->DBG_ShowInfo();

	/*auto crateMesh = Mesh::Create("crateMesh", "temp/cube.obj");
	auto ballMesh = Mesh::Create("ballMesh", "temp/ball.obj");
	auto floorMesh = Mesh::Create("floorMesh", "temp/plane.obj");
	auto lightbulbMesh = Mesh::Create("lightbulbMesh", "temp/plane.obj");

	auto crateTex = Texture2D::Create("crateTex", "crate.png");
	auto ballTex = Texture2D::Create("ballTex", "not-a-real-texture.png");
	auto floorTex = Texture2D::Create("floorTex", "floor.png");
	auto lightbulbTex = Texture2D::Create("lightbulbTex", "lightbulb.png");

	auto crateMat = Material::Create("crateMat", diffuseShader, crateTex);
	auto ballMat = Material::Create("ballMat", diffuseShader, ballTex);
	auto floorMat = Material::Create("floorMat", diffuseShader, floorTex);
	auto lightbulbMat = std::make_shared<Material>("lightbulb", diffuseShader, lightbulbTex);

	auto crate = std::make_shared<GameObject>(crateMesh, crateMat);
	auto ball = std::make_shared<GameObject>(ballMesh, ballMat);
	auto floor = std::make_shared<GameObject>(floorMesh, floorMat);
	auto lightbulb = std::make_shared<GameObject>(lightbulbMesh, lightbulbMat);

	auto crateMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto ballMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto floorMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto lightbulbMat_old = std::make_shared<Material_OLD>(0.6f, 1.0f, 1.0f);
	
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	crate->SetMaterial_OLD(crateMat_old);
	scene->AddObject(crate);
	ObjectManager::AddObject("crate", crate);
	
	ball->SetPosition(glm::vec3(0.0f, 15.0f, -15.0f));
	ball->SetMaterial_OLD(ballMat_old);
	scene->AddObject(ball);
	ObjectManager::AddObject("ball", ball);

	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	floor->SetMaterial_OLD(floorMat_old);
	scene->AddObject(floor);
	ObjectManager::AddObject("floor", floor);

	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 0.3f, 0.9f, 0.4f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 0.0f);
	scene->SetLocalLight(localLight);
	auto globalLightPos = glm::vec3{ 40.0f, 40.0f, 40.0f };
	auto globalLightColor = glm::vec3{ 0.0f, 0.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 0.5f);
	scene->SetGlobalLight(globalLight);

	lightbulb->SetPosition(localLightPos);
	lightbulb->SetMaterial_OLD(lightbulbMat_old);
	scene->AddObject(lightbulb);
	ObjectManager::AddObject("lightbulb", lightbulb);*/
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

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 500.0f, 1200.0f / 800.0f);
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });

	auto diffuseScene = std::make_shared<Scene>();
	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 0.3f, 0.9f, 0.4f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 0.0f);
	diffuseScene->SetLocalLight(localLight);
	auto globalLightPos = glm::vec3{ 40.0f, 40.0f, 40.0f };
	auto globalLightColor = glm::vec3{ 0.0f, 0.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 0.5f);
	diffuseScene->SetGlobalLight(globalLight);

	//SetUp3DScene(diffuseScene);
	
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
	//auto diffuseShader2 = Shader::Get("diffuse");

	ge.SetCamera(cam);
	ge.Run3D(reg, diffuseScene, diffuseShader);

	return 0;
}
