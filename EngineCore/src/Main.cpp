#define NOMINMAX //this is needed so that rapidjson doesn't shit the bed, idk why

#include "EventManager.h"
#include "FileSystem.h"
#include "Generate.h"
#include "GraphicsContext.h"
#include "GraphicsEnvironment.h"
#include "JsonUtils.h"
#include "ScriptManager.h"
#include "ui/UISystem.h"

#include <Windows.h>

#ifdef CreateWindow //fuck you microsoft, i want this function name
#undef CreateWindow
#endif

using namespace Cicada;

static void SetUp3DScene(std::shared_ptr<Scene>& scene) {

	auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");
	diffuseShader->DBG_ShowInfo();

	auto dummyMesh = std::make_shared<Mesh>(12);
	dummyMesh->Setup();
	dummyMesh->LoadObj("chicken brent.obj");

	auto crateMesh = Generate::CuboidWithNormals(10.0, 10.0, 10.0, 1.0, 1.0, { 1, 1, 1, 1 });
	auto moverMesh = Generate::PolarSphereWithNormals(1, 4, 18);
	auto floorMesh = Generate::PlaneXZWithNormals(100, 100, 12, 12, { 1, 1, 1, 1 });
	auto lightbulbMesh = Generate::PlaneXYWithNormals(1, 1, 1, 1, { 1, 1, 1, 1 });

	auto dummyTex = std::make_shared<Texture2D>("br0_tex00.png");
	auto crateTex = std::make_shared<Texture2D>("crate.png");
	auto moverTex = std::make_shared<Texture2D>("not-a-real-texture.png");
	auto floorTex = std::make_shared<Texture2D>("floor.png");
	auto lightbulbTex = std::make_shared<Texture2D>("lightbulb.png");

	auto dummyMat = std::make_shared<Material>("dummy", diffuseShader, dummyTex);
	auto crateMat = std::make_shared<Material>("crate", diffuseShader, crateTex);
	auto moverMat = std::make_shared<Material>("mover", diffuseShader, moverTex);
	auto floorMat = std::make_shared<Material>("floor", diffuseShader, floorTex);
	auto lightbulbMat = std::make_shared<Material>("lightbulb", diffuseShader, lightbulbTex);

	auto dummy = std::make_shared<GameObject>(dummyMesh, dummyMat);
	auto crate = std::make_shared<GameObject>(crateMesh, crateMat);
	auto mover = std::make_shared<GameObject>(moverMesh, moverMat);
	auto floor = std::make_shared<GameObject>(floorMesh, floorMat);
	auto lightbulb = std::make_shared<GameObject>(lightbulbMesh, lightbulbMat);

	auto dummyMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto crateMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto moverMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto floorMat_old = std::make_shared<Material_OLD>(0.2f, 1.0f, 1.0f);
	auto lightbulbMat_old = std::make_shared<Material_OLD>(0.6f, 1.0f, 1.0f);


	dummyTex->Upload();
	dummy->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	dummy->SetMaterial_OLD(dummyMat_old);
	scene->AddObject(dummy);
	ObjectManager::AddObject("dummy", dummy);
	
	crateTex->Upload();
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	crate->SetMaterial_OLD(crateMat_old);
	scene->AddObject(crate);
	ObjectManager::AddObject("crate", crate);
	
	moverTex->Upload();
	mover->SetPosition(glm::vec3(0.0f, 15.0f, -15.0f));
	mover->SetMaterial_OLD(moverMat_old);
	scene->AddObject(mover);
	ObjectManager::AddObject("mover", mover);

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

	ScriptManager::Init();
	ScriptManager::LoadScript("test.lua");

	EventManager::TriggerEvent("OnStart");

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

	SetUp3DScene(diffuseScene);
	Renderer::UploadResources(diffuseScene);

	rapidjson::Document sceneSchema, sceneJson;
	JsonUtils::ReadJson("scene_schema.json", sceneSchema);
	JsonUtils::ReadJson("test_scene.json", sceneJson);

	bool ye = JsonUtils::ValidateAgainstSchema(sceneSchema, sceneJson);

	if (ye)
		Logger::Write(LogEntry::Level::Info, "All clear!");

	auto& objectsJson = sceneJson["gameObjects"];

	auto diffuseShader = Shader::Get("diffuse");

	ge.SetCamera(cam);
	ge.Run3D(diffuseScene, diffuseShader);

	return 0;
}
