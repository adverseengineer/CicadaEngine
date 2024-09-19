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
	/*litShader->AddUniform("world");
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
	litShader->AddUniform("tex");*/

	auto litScene = std::make_shared<Scene>();
	ge.AddRenderer("lit", litShader, litScene);
	#pragma endregion
	
	#pragma region SetUpFlatShader
	vertText.ReadAllLines("texture.vert.glsl");
	fragText.ReadAllLines("texture.frag.glsl");
	auto flatShader = std::make_shared<Shader>(vertText.GetContents(), fragText.GetContents());
	/*flatShader->AddUniform("world");
	flatShader->AddUniform("view");
	flatShader->AddUniform("projection");
	flatShader->AddUniform("texUnit");*/
	auto flatScene = std::make_shared<Scene>();
	ge.AddRenderer("flat", flatShader, flatScene);
	#pragma endregion

	#pragma region SetUpBasicShader
	vertText.ReadAllLines("basic.vert.glsl");
	fragText.ReadAllLines("basic.frag.glsl");
	auto basicShader = std::make_shared<Shader>(vertText.GetContents(), fragText.GetContents());
	/*basicShader->AddUniform("world");
	basicShader->AddUniform("view");
	basicShader->AddUniform("projection");*/
	auto basicScene = std::make_shared<Scene>();
	ge.AddRenderer("basic", basicShader, basicScene);
	#pragma endregion

	#pragma region SetUpLighting
	auto localLightPos = glm::vec3{ 50.0f, 10.0f, 50.0f };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 0.0f);
	litScene->SetLocalLight(localLight);
	auto globalLightPos = glm::vec3{ 50.0f, 10.0f, 50.0f };
	auto globalLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 0.5f);
	litScene->SetGlobalLight(globalLight);

	auto sphereVBuf = Generate::QuadSphere(4, 10);
	//auto sphereVBuf = Generate::PolarSphere(4, 24, 24);
	//sphereVBuf->SetPrimitiveType(GL_LINE_STRIP);

	auto sphereTex = std::make_shared<Texture>();
	sphereTex->LoadTextureDataFromFile("globe.png");
	sphereVBuf->SetTexture(sphereTex);
	auto sphere = std::make_shared<GraphicsObject>();
	sphere->SetVertexBuffer(sphereVBuf);
	sphere->SetPosition({ 0.0f, 0.0f, 0.0f });
	auto sphereMat = std::make_shared<Material>(0.0f, 1.0f, 1.0f);
	sphere->SetMaterial(sphereMat);
	litScene->AddObject(sphere);
	ge.AddObject("sphere", sphere);

	auto skyDomeVBuf = Generate::QuadSphere(-50, 35, { 0.4f, 0.4f, 0.4f, 1.0f });
	auto skyDomeTex = std::make_shared<Texture>();
	skyDomeTex->LoadTextureDataFromFile("space.png");
	skyDomeVBuf->SetTexture(skyDomeTex);
	auto skyDome = std::make_shared<GraphicsObject>();
	skyDome->SetVertexBuffer(skyDomeVBuf);
	skyDome->SetPosition({ 0.0f, 5.0f, 0.0f });
	auto skyDomeMat = std::make_shared<Material>(1.0f, 9.0f, 1.0f);
	skyDome->SetMaterial(skyDomeMat);
	litScene->AddObject(skyDome);
	ge.AddObject("skyDome", skyDome);

	auto hitboxVisualization = Generate::CuboidWithNormals(0.2f,0.2f,0.2f);

	
	std::shared_ptr<GraphicsObject> continents[7];
	std::shared_ptr<VertexBuffer> continentVBufs[7];
	for (std::size_t i = 0; i < 7; i++) {
		continents[i] = std::make_shared<GraphicsObject>();
		continentVBufs[i] = Generate::CuboidWithNormals(0.1f, 0.1f, 0.1f);

		continents[i]->SetVertexBuffer(continentVBufs[i]);
		continents[i]->SetPosition(glm::vec3(0.0f));
		continents[i]->SetMaterial(std::make_shared<Material>(1.0f, 1.0f, 1.0f));
		litScene->AddObject(continents[i]);
		ge.AddObject("continent #" + i, continents[i]);

		sphere->AddChild(continents[i]);
	}
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

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 5000.0f, 1200.0f / 800.0f);
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });
	ge.SetCamera(cam);
	ge.SetClearColor(glm::vec3{ 0.8f, 0.8f, 0.8f });

	ge.Run3D();

	return 0;
}
