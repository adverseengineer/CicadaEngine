#include "Generate.h"
#include "GraphicsEnvironment.h"
#include <Windows.h>

static void SetUp3DScene(GraphicsEnvironment& ge) {

	#pragma region SetUpLightShader
	std::string vertexSource;
	std::string fragmentSource;
	Util::ReadFileToString("lighting.vert.glsl", vertexSource);
	Util::ReadFileToString("lighting.frag.glsl", fragmentSource);
	std::shared_ptr<Shader> diffuseShader = std::make_shared<Shader>(vertexSource, fragmentSource);
	ShaderManager::AddShader("diffuse", diffuseShader);

	auto diffuseScene = std::make_shared<Scene>();
	ge.AddRenderer("diffuse", diffuseShader, diffuseScene);
	#pragma endregion

	#pragma region SetUpDummyCube
	auto dummyVBuf = Generate::CuboidWithNormals(6, 2, 5);
	auto dummyTex = std::make_shared<Texture>("not-a-real-file.png");
	auto dummy = std::make_shared<GameObject>();
	dummy->SetTexture(dummyTex);
	dummy->SetVertexBuffer(dummyVBuf);
	dummy->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	auto dummyMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	dummy->SetMaterial(dummyMat);
	diffuseScene->AddObject(dummy);
	ge.AddObject("dummy", dummy);
	
	auto dummyBB = std::make_shared<BoundingBox>();
	dummyBB->Create(6, 2, 5);
	dummyBB->SetReferenceFrame(dummy->GetLocalReferenceFrame());
	dummy->SetBoundingBox(dummyBB);
	#pragma endregion

	#pragma region SetUpCrate
	auto crateTex = std::make_shared<Texture>("crate.png");
	auto crateVBuf = Generate::CuboidWithNormals(10, 10, 10, { 1, 1, 1, 1 }, { 1, 1 });
	auto crate = std::make_shared<GameObject>();
	crate->SetTexture(crateTex);
	crate->SetVertexBuffer(crateVBuf);
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	auto crateMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	crate->SetMaterial(crateMat);
	diffuseScene->AddObject(crate);
	ge.AddObject("crate", crate);

	auto crateBB = std::make_shared<BoundingBox>();
	crateBB->Create(10,10,10);
	crateBB->SetReferenceFrame(crate->GetLocalReferenceFrame());
	crate->SetBoundingBox(crateBB);
	#pragma endregion

	#pragma region SetUpMover
	auto moverTex = std::make_shared<Texture>("gw.png");
	auto moverVBuf = Generate::CuboidWithNormals(1.0f, 10.0f, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 10.0f });
	auto mover = std::make_shared<GameObject>();
	mover->SetTexture(moverTex);
	mover->SetVertexBuffer(moverVBuf);
	mover->SetPosition(glm::vec3(40.0f, 10.0f, 40.0f));
	auto moverMat = std::make_shared<Material>(0.2f, 1.0f, 1.0f);
	mover->SetMaterial(moverMat);
	diffuseScene->AddObject(mover);
	ge.AddObject("mover", mover);

	auto moverBB = std::make_shared<BoundingBox>();
	moverBB->Create(1, 10, 1);
	moverBB->SetReferenceFrame(mover->GetLocalReferenceFrame());
	mover->SetBoundingBox(moverBB);
	#pragma endregion

	#pragma region SetUpFloor
	auto floorVBuf = Generate::PlaneXZWithNormals(100, 100, { 1, 1, 1, 1 }, { 12, 12 });
	auto floorTex = std::make_shared<Texture>("floor.png");
	auto floor = std::make_shared<GameObject>();
	floor->SetTexture(floorTex);
	floor->SetVertexBuffer(floorVBuf);
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	auto floorMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	floor->SetMaterial(floorMat);
	diffuseScene->AddObject(floor);
	ge.AddObject("floor", floor);
	#pragma endregion

	#pragma region SetUpLighting
	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 0.0f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 0.0f);
	diffuseScene->SetLocalLight(localLight);
	auto globalLightPos = glm::vec3{ 20.0f, 20.0f, 20.0f };
	auto globalLightColor = glm::vec3{ 0.0f, 0.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 0.5f);
	diffuseScene->SetGlobalLight(globalLight);

	#pragma region Lightbulb
	//auto lightbulbVBuf = Generate::PlaneXY(1, 1, { 1, 1, 1 }, { 1, 1 });
	auto lightbulbVBuf = Generate::PlaneXYWithNormals(1, 1, { 1, 1, 1, 1 }, { 1, 1 });
	auto lightbulbTex = std::make_shared<Texture>("lightbulb.png");
	auto lightbulb = std::make_shared<GameObject>();
	lightbulb->SetTexture(lightbulbTex);
	lightbulb->SetVertexBuffer(lightbulbVBuf);
	lightbulb->SetPosition(localLightPos);
	auto lightbulbMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	lightbulb->SetMaterial(lightbulbMat);
	//flatScene->AddObject(lightbulb);
	diffuseScene->AddObject(lightbulb);
	ge.AddObject("lightbulb", lightbulb);
	#pragma endregion

	auto dummyHLBehavior = std::make_shared<HighlightBehavior>();
	auto crateHLBehavior = std::make_shared<HighlightBehavior>();
	auto moverHLBehavior = std::make_shared<HighlightBehavior>();
	auto moverTranslateBehavior = std::make_shared<TranslateAnimation>();
	auto moverRotateBehavior = std::make_shared<RotateAnimation>();

	dummyHLBehavior->SetObject(dummy);
	crateHLBehavior->SetObject(crate);
	moverHLBehavior->SetObject(mover);
	moverTranslateBehavior->SetObject(mover);
	moverRotateBehavior->SetObject(mover);
	dummy->AddBehavior("highlight", dummyHLBehavior);
	crate->AddBehavior("highlight", crateHLBehavior);
	mover->AddBehavior("highlight", moverHLBehavior);
	mover->AddBehavior("translate", moverTranslateBehavior);
	mover->AddBehavior("rotate", moverRotateBehavior);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	GraphicsEnvironment ge;
	ge.Init(4, 3);
	bool created = ge.SetWindow(1200, 800, "ETSU Computing Interactive Graphics");
	if (created == false)
		return -1;
	bool loaded = ge.InitGlad();
	if (loaded == false)
		return -1;

	ge.SetupGraphics();

	SetUp3DScene(ge);
	ge.StaticAllocate();

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 500.0f, 1200.0f / 800.0f);
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });
	ge.SetCamera(cam);

	ge.Run3D();

	return 0;
}
