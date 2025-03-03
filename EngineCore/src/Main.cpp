#define NOMINMAX //this is needed so that rapidjson doesn't shit the bed, idk why

#include "EventManager.h"
#include "FileSystem.h"
#include "GraphicsContext.h"
#include "GraphicsEnvironment.h"
#include "JsonUtils.h"
#include "ManagedObject.h"
#include "ScriptManager.h"
#include "ui/UISystem.h"

#include <Windows.h>

#ifdef CreateWindow //fuck you microsoft, i want this function name
#undef CreateWindow
#endif

using namespace Cicada;

static void SetUp3DScene(std::shared_ptr<Scene>& scene) {

	auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");
	//diffuseShader->DBG_ShowInfo();

	auto crateMesh = Mesh::Create("crateMesh");
	crateMesh->Setup();
	crateMesh->LoadObj("temp/cube.obj");

	auto ballMesh = Mesh::Create("ballMesh");
	ballMesh->Setup();
	ballMesh->LoadObj("temp/uvsphere.obj");

	auto floorMesh = Mesh::Create("floorMesh");
	floorMesh->Setup();
	floorMesh->LoadObj("temp/plane.obj");

	auto lightbulbMesh = Mesh::Create("lightbulbMesh");
	lightbulbMesh->Setup();
	lightbulbMesh->LoadObj("temp/plane.obj");

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
	
	crateTex->Upload();
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	crate->SetMaterial_OLD(crateMat_old);
	scene->AddObject(crate);
	ObjectManager::AddObject("crate", crate);
	
	ballTex->Upload();
	ball->SetPosition(glm::vec3(0.0f, 15.0f, -15.0f));
	ball->SetMaterial_OLD(ballMat_old);
	scene->AddObject(ball);
	ObjectManager::AddObject("ball", ball);

	floorTex->Upload();
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

	lightbulbTex->Upload();
	lightbulb->SetPosition(localLightPos);
	lightbulb->SetMaterial_OLD(lightbulbMat_old);
	scene->AddObject(lightbulb);
	ObjectManager::AddObject("lightbulb", lightbulb);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	//ScriptManager::Init();
	//ScriptManager::LoadScript("test.lua");

	//EventManager::TriggerEvent("OnStart");

	auto& gc = GraphicsContext::Instance();
	gc.CreateWindow(1200, 800, "Cicada Engine");
	gc.InitGlad();
	gc.Configure();

	{
		auto mgdShad = Shader::Create("mgdShad", "basic.vert.glsl", "basic.frag.glsl");
		auto mgdTex = Texture2D::Create("mgdTex", "gw.png");
		auto mgdMat = Material::Create("mgdMat", mgdShad, mgdTex);

		//auto mesh = Mesh::Create("managedMesh");
	}

	auto& ge = GraphicsEnvironment::Instance();

	//need to initialize UI after window is made
	auto& ui = UI::UISystem::Instance();

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 500.0f, 1200.0f / 800.0f);
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });

	auto diffuseScene = std::make_shared<Scene>();

	SetUp3DScene(diffuseScene);
	Renderer::UploadResources(diffuseScene);

	//rapidjson::Document sceneSchema, sceneJson;
	//JsonUtils::ReadJson("scene_schema.json", sceneSchema);
	//JsonUtils::ReadJson("test_scene.json", sceneJson);

	//bool ye = JsonUtils::ValidateAgainstSchema(sceneSchema, sceneJson);

	//if (ye)
		//Logger::Write(LogEntry::Level::Info, "All clear!");

	//auto& objectsJson = sceneJson["gameObjects"];

	auto diffuseShader = Shader::Get("diffuse");

	ge.SetCamera(cam);
	ge.Run3D(diffuseScene, diffuseShader);

	return 0;
}
