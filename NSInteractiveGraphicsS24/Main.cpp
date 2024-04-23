//#include "Behavior.h"
#include "Generate.h"
#include "GraphicsEnvironment.h"
#include "TextFile.h"
#include <Windows.h>

static void SetUp3DScene(GraphicsEnvironment& ge) {

	#pragma region SetUpLightShader
	TextFile vertText;
	vertText.ReadAllLines("lighting.vert.glsl");
	TextFile fragText;
	fragText.ReadAllLines("lighting.frag.glsl");
	auto litShader = std::make_shared<Shader>(vertText.GetContents(), fragText.GetContents());
	litShader->AddUniform("world");
	litShader->AddUniform("view");
	litShader->AddUniform("projection");
	litShader->AddUniform("materialAmbientIntensity");
	litShader->AddUniform("materialSpecularIntensity");
	litShader->AddUniform("materialShininess");
	litShader->AddUniform("globalLightPosition");
	litShader->AddUniform("globalLightColor");
	litShader->AddUniform("globalLightIntensity");
	litShader->AddUniform("localLightPosition");
	litShader->AddUniform("localLightColor");
	litShader->AddUniform("localLightIntensity");
	litShader->AddUniform("localLightAttenuationCoef");
	litShader->AddUniform("viewPosition");
	litShader->AddUniform("tex");
	auto litScene = std::make_shared<Scene>();
	ge.AddRenderer("lit", litShader, litScene);
	#pragma endregion
	
	#pragma region SetUpFlatShader
	vertText.ReadAllLines("texture.vert.glsl");
	fragText.ReadAllLines("texture.frag.glsl");
	auto flatShader = std::make_shared<Shader>(vertText.GetContents(), fragText.GetContents());
	flatShader->AddUniform("world");
	flatShader->AddUniform("view");
	flatShader->AddUniform("projection");
	flatShader->AddUniform("texUnit");
	auto flatScene = std::make_shared<Scene>();
	ge.AddRenderer("flat", flatShader, flatScene);
	#pragma endregion

	#pragma region SetUpBasicShader
	vertText.ReadAllLines("basic.vert.glsl");
	fragText.ReadAllLines("basic.frag.glsl");
	auto basicShader = std::make_shared<Shader>(vertText.GetContents(), fragText.GetContents());
	basicShader->AddUniform("world");
	basicShader->AddUniform("view");
	basicShader->AddUniform("projection");
	auto basicScene = std::make_shared<Scene>();
	ge.AddRenderer("basic", basicShader, basicScene);
	#pragma endregion

	#pragma region SetUpDummyCube
	auto dummyVBuf = Generate::CuboidWithNormals(6, 2, 5);
	auto dummyTex = std::make_shared<Texture>();
	dummyTex->LoadTextureDataFromFile("not-a-real-file.png");
	dummyVBuf->SetTexture(dummyTex);
	auto dummy = std::make_shared<GraphicsObject>();
	dummy->SetVertexBuffer(dummyVBuf);
	dummy->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	auto dummyMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	dummy->SetMaterial(dummyMat);
	litScene->AddObject(dummy);
	ge.AddObject("dummy", dummy);
	
	auto dummyBB = std::make_shared<BoundingBox>();
	dummyBB->Create(6, 2, 5);
	dummyBB->SetReferenceFrame(dummy->GetLocalReferenceFrame());
	dummy->SetBoundingBox(dummyBB);
	#pragma endregion

	#pragma region SetUpCrate
	auto crateTex = std::make_shared<Texture>();
	crateTex->LoadTextureDataFromFile("crate.png");
	auto crateVBuf = Generate::CuboidWithNormals(10, 10, 10, { 1, 1, 1, 1 }, { 1, 1 });
	crateVBuf->SetTexture(crateTex);
	auto crate = std::make_shared<GraphicsObject>();
	crate->SetVertexBuffer(crateVBuf);
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	auto crateMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	crate->SetMaterial(crateMat);
	litScene->AddObject(crate);
	ge.AddObject("crate", crate);

	auto crateBB = std::make_shared<BoundingBox>();
	crateBB->Create(10,10,10);
	crateBB->SetReferenceFrame(crate->GetLocalReferenceFrame());
	crate->SetBoundingBox(crateBB);
	#pragma endregion

	#pragma region SetUpFloor
	auto floorVBuf = Generate::PlaneXZWithNormals(100, 100, { 1, 1, 1, 1 }, { 12, 12 });
	auto floorTex = std::make_shared<Texture>();
	//floorTex->LoadTextureDataFromFile("floor.png");
	floorTex->LoadTextureDataFromFile("space.png");
	floorVBuf->SetTexture(floorTex);
	auto floor = std::make_shared<GraphicsObject>();
	floor->SetVertexBuffer(floorVBuf);
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	auto floorMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	floor->SetMaterial(floorMat);
	litScene->AddObject(floor);
	ge.AddObject("floor", floor);
	#pragma endregion

	#pragma region SetUpLighting
	auto localLightPos = glm::vec3{ 0, 50.0f, 50.0f };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 0.0f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 0.0f);
	litScene->SetLocalLight(localLight);
	auto globalLightPos = glm::vec3{ 20.0f, 20.0f, 20.0f };
	auto globalLightColor = glm::vec3{ 0.0f, 0.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 0.5f);
	litScene->SetGlobalLight(globalLight);

	auto sphereVBuf = Generate::QuadSphere(4, 40, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	auto sphereTex = std::make_shared<Texture>();
	sphereTex->LoadTextureDataFromFile("cat.png");
	sphereVBuf->SetTexture(sphereTex);
	auto sphere = std::make_shared<GraphicsObject>();
	sphere->SetVertexBuffer(sphereVBuf);
	sphere->SetPosition({ 0.0f, 5.0f, 0.0f });
	auto sphereMat = std::make_shared<Material>(0.1f, 1.0f, 1.0f);
	sphere->SetMaterial(sphereMat);
	litScene->AddObject(sphere);
	ge.AddObject("sphere", sphere);

	auto sphereBB = std::make_shared<BoundingBox>();
	sphereBB->Create(4, 4, 4);
	sphereBB->SetReferenceFrame(sphere->GetLocalReferenceFrame());
	sphere->SetBoundingBox(sphereBB);

	auto skyDomeVBuf = Generate::QuadSphere(-100, 40, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	auto skyDomeTex = std::make_shared<Texture>();
	skyDomeTex->LoadTextureDataFromFile("space.png");
	skyDomeVBuf->SetTexture(skyDomeTex);
	auto skyDome = std::make_shared<GraphicsObject>();
	skyDome->SetVertexBuffer(skyDomeVBuf);
	skyDome->SetPosition({ 0.0f, 5.0f, 0.0f });
	auto skyDomeMat = std::make_shared<Material>(0.1f, 1.0f, 1.0f);
	skyDome->SetMaterial(skyDomeMat);
	litScene->AddObject(skyDome);
	ge.AddObject("skyDome", skyDome);

	auto dummyHLBehavior = std::make_shared<HighlightBehavior>();
	auto crateHLBehavior = std::make_shared<HighlightBehavior>();
	
	dummyHLBehavior->SetObject(dummy);
	crateHLBehavior->SetObject(crate);
	dummy->AddBehavior("highlight", dummyHLBehavior);
	crate->AddBehavior("highlight", crateHLBehavior);
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
