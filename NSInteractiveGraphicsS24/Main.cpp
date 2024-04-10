#include "Generate.h"
#include "GraphicsEnvironment.h"
#include "RotateAnimation.h"
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
	auto dummyCubeVBuf = Generate::CuboidWithNormals(6, 2, 5);
	auto dummyTex = std::make_shared<Texture>();
	dummyTex->LoadTextureDataFromFile("not-a-real-file.png");
	dummyCubeVBuf->SetTexture(dummyTex);
	auto dummyCube = std::make_shared<GraphicsObject>();
	dummyCube->SetVertexBuffer(dummyCubeVBuf);
	dummyCube->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	auto mat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	dummyCube->SetMaterial(mat);
	auto rotateAnimation = std::make_shared<RotateAnimation>(glm::vec3{ 0.0f,1.0f,0.0f }, 90.0f);
	rotateAnimation->SetObject(dummyCube);
	dummyCube->SetAnimation(rotateAnimation);
	litScene->AddObject(dummyCube);
	ge.AddObject("dummy cube", dummyCube);
	#pragma endregion

	#pragma region SetUpCrate
	auto crateTex = std::make_shared<Texture>();
	crateTex->LoadTextureDataFromFile("crate.png");
	auto crateVBuf = Generate::CuboidWithNormals(10, 10, 10, { 1, 1, 1, 1 }, { 1, 1 });
	crateVBuf->SetTexture(crateTex);
	auto crate = std::make_shared<GraphicsObject>();
	crate->SetVertexBuffer(crateVBuf);
	crate->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	crate->SetMaterial(mat);
	litScene->AddObject(crate);
	ge.AddObject("crate", crate);
	#pragma endregion

	#pragma region SetUpFloor
	auto floorVBuf = Generate::PlaneXZWithNormals(100, 100, { 1, 1, 1, 1 }, { 8, 8 });
	auto floorTex = std::make_shared<Texture>();
	floorTex->LoadTextureDataFromFile("floor.png");
	floorVBuf->SetTexture(floorTex);
	auto floor = std::make_shared<GraphicsObject>();
	floor->SetVertexBuffer(floorVBuf);
	floor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	floor->SetMaterial(mat);
	litScene->AddObject(floor);
	ge.AddObject("floor", floor);
	#pragma endregion

	#pragma region SetUpLighting
	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 1.0f);
	litScene->SetLocalLight(localLight);
	auto globalLightPos = glm::vec3{ 20.0f, 20.0f, 20.0f };
	auto globalLightColor = glm::vec3{ 1.0f, 0.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 0.2f, 1.0f);
	litScene->SetGlobalLight(globalLight);

	#pragma region Lightbulb
	//auto lightbulbVBuf = Generate::PlaneXY(1, 1, { 1, 1, 1 }, { 1, 1 });
	auto lightbulbVBuf = Generate::PlaneXYWithNormals(1, 1, { 1, 1, 1, 1 }, { 1, 1 });
	auto lightbulbTex = std::make_shared<Texture>();
	lightbulbTex->LoadTextureDataFromFile("lightbulb.png");
	lightbulbVBuf->SetTexture(lightbulbTex);
	auto lightbulb = std::make_shared<GraphicsObject>();
	lightbulb->SetVertexBuffer(lightbulbVBuf);
	lightbulb->SetPosition(localLightPos);
	auto fullbrightMat = std::make_shared<Material>(0.6f, 1.0f, 1.0f);
	lightbulb->SetMaterial(fullbrightMat);
	//flatScene->AddObject(lightbulb);
	litScene->AddObject(lightbulb);
	ge.AddObject("lightbulb", lightbulb);
	#pragma endregion

	auto circleVBuf = Generate::LineCircleVertices(5, 12, glm::vec3(1.0f,1.0f,0.0f));
	circleVBuf->SetPrimitiveType(GL_LINES);
	auto circleIBuf = Generate::LineCircleIndices(12, true);
	auto circle = std::make_shared<GraphicsObject>();
	circle->SetVertexBuffer(circleVBuf);
	circle->SetIndexBuffer(circleIBuf);
	circle->SetPosition({ 0.0f, 1.0f, 7.0f });
	basicScene->AddObject(circle);
	ge.AddObject("circle", circle);

	auto cylinderVBuf = Generate::LineCylinderVertices(5, 4, 24);
	cylinderVBuf->SetPrimitiveType(GL_LINES);
	auto cylinderIBuf = Generate::LineCylinderIndices(4, true);
	auto cylinder = std::make_shared<GraphicsObject>();
	cylinder->SetVertexBuffer(cylinderVBuf);
	cylinder->SetIndexBuffer(cylinderIBuf);
	cylinder->SetPosition({ -10.0f, 10.0f, 10.0f });
	basicScene->AddObject(cylinder);
	ge.AddObject("cylinder", cylinder);
#pragma endregion
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

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

	auto cam = std::make_shared<Camera>();
	ge.SetCamera(cam);

	ge.Run3D();

	return 0;
}
