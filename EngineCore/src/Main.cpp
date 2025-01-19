#define NOMINMAX //this is needed so that rapidjson doesn't shit the bed, idk why

#include "Generate.h"
#include "GraphicsEnvironment.h"
#include <Windows.h>

#include "EventManager.h"

#include "JsonUtils.h"

static void SetUp3DScene(GraphicsEnvironment& ge, std::shared_ptr<Scene>& scene) {

	std::string vertexSource, fragmentSource;
	Util::ReadFileToString("diffuse.vert.glsl", vertexSource);
	Util::ReadFileToString("diffuse.frag.glsl", fragmentSource);

	std::shared_ptr<Shader> diffuseShader = std::make_shared<Shader>(vertexSource, fragmentSource);
	ShaderManager::AddShader("diffuse", diffuseShader);
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

	auto dummyMat = std::make_shared<Material>(diffuseShader, dummyTex);
	auto crateMat = std::make_shared<Material>(diffuseShader, crateTex);
	auto moverMat = std::make_shared<Material>(diffuseShader, moverTex);
	auto floorMat = std::make_shared<Material>(diffuseShader, floorTex);
	auto lightbulbMat = std::make_shared<Material>(diffuseShader, lightbulbTex);

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
	ge.AddObject("dummy", dummy);
	
	crateTex->Upload();
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	crate->SetMaterial_OLD(crateMat_old);
	scene->AddObject(crate);
	ge.AddObject("crate", crate);
	
	moverTex->Upload();
	mover->SetPosition(glm::vec3(0.0f, 15.0f, -15.0f));
	mover->SetMaterial_OLD(moverMat_old);
	scene->AddObject(mover);
	ge.AddObject("mover", mover);

	floorTex->Upload();
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	floor->SetMaterial_OLD(floorMat_old);
	scene->AddObject(floor);
	ge.AddObject("floor", floor);

	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 0.0f };
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
	ge.AddObject("lightbulb", lightbulb);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	//EventManager& em = EventManager::Instance();
	EventManager::Init();
	EventManager::LoadScript("test.lua");

	EventManager::TriggerEvent("OnStart");

	GraphicsEnvironment ge;
	bool created = ge.SetWindow(1200, 800, "ETSU Computing Interactive Graphics");
	if (created == false)
		return -1;
	bool loaded = ge.InitGlad();
	if (loaded == false)
		return -1;

	ge.SetupGraphics();

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 500.0f, 1200.0f / 800.0f);
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });

	auto diffuseScene = std::make_shared<Scene>();

	SetUp3DScene(ge, diffuseScene);
	Renderer::UploadResources(diffuseScene);

	rapidjson::Document sceneSchema, sceneJson;
	JsonUtils::ReadJson("scene_schema.json", sceneSchema);
	JsonUtils::ReadJson("test_scene.json", sceneJson);

	bool ye = JsonUtils::ValidateAgainstSchema(sceneSchema, sceneJson);

	if (ye)
		Util::Log(LogEntry::Severity::Info, "All clear!");

	auto& objectsJson = sceneJson["gameObjects"];

	ge.SetCamera(cam);
	ge.Run3D(diffuseScene, ShaderManager::GetShader("diffuse"));

	return 0;
}
