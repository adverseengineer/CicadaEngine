#include "Generate.h"
#include "GraphicsEnvironment.h"
#include "GraphicsObject.h"
#include "Renderer.h"
#include "RotateAnimation.h"
#include "Scene.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "TextFile.h"
#include "VertexBuffer.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>

static void SetUp3DScene2(std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene, GraphicsEnvironment& ge) {

	scene = std::make_shared<Scene>();

	#pragma region SetUpShader
	//set up the shader that will be used for textured objects
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("lighting.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("lighting.frag.glsl");
	shader = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	shader->AddUniform("texUnit");
	shader->AddUniform("materialAmbientIntensity");
	shader->AddUniform("materialSpecularIntensity");
	shader->AddUniform("materialShininess");
	shader->AddUniform("globalLightPosition");
	shader->AddUniform("globalLightColor");
	shader->AddUniform("globalLightIntensity");
	shader->AddUniform("localLightPosition");
	shader->AddUniform("localLightColor");
	shader->AddUniform("localLightIntensity");
	shader->AddUniform("localLightAttenuationCoef");
	shader->AddUniform("viewPosition");
	shader->AddUniform("tex");
	#pragma endregion

	#pragma region RawTexture
	auto rawTex = std::make_shared<Texture>();
	rawTex->SetDimensions(2, 2);
	unsigned char rawTextureData[16] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
	rawTex->SetTextureData(16, rawTextureData);
	rawTex->SetWrapS(GL_REPEAT);
	rawTex->SetWrapT(GL_REPEAT);
	rawTex->SetMagFilter(GL_NEAREST);
	#pragma endregion

	#pragma region RawTexture2
	auto rawTex2 = std::make_shared<Texture>();
	rawTex2->SetDimensions(2, 2);
	unsigned char rawTextureData2[16] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
	rawTex2->SetTextureData(16, rawTextureData2);
	rawTex2->SetWrapS(GL_REPEAT);
	rawTex2->SetWrapT(GL_REPEAT);
	rawTex2->SetMagFilter(GL_NEAREST);
	#pragma endregion

	auto mat1 = std::make_shared<Material>(1.0f, 1.0f, 1.0f);

	#pragma region SetUpCube1
	auto cubeVBuf1 = Generate::CuboidWithNormals(10, 10, 10, { 1, 1, 1, 1 }, { 1, 1 });
	cubeVBuf1->SetTexture(rawTex);
	auto cube1 = std::make_shared<GraphicsObject>();
	cube1->SetVertexBuffer(cubeVBuf1);
	cube1->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	cube1->SetMaterial(mat1);
	scene->AddObject(cube1);
	ge.AddObject("cube 1", cube1);

	auto rotateAnimation = std::make_shared<RotateAnimation>(glm::vec3{0.0f,1.0f,0.0f}, 0.0f);
	rotateAnimation->SetObject(cube1);
	cube1->SetAnimation(rotateAnimation);
	#pragma endregion

	#pragma region SetUpCube2
	auto crateTex = std::make_shared<Texture>();
	crateTex->LoadTextureDataFromFile("crate.png");
	auto cubeVBuf2 = Generate::CuboidWithNormals(6, 2, 5);
	cubeVBuf2->SetTexture(crateTex);
	auto cube2 = std::make_shared<GraphicsObject>();
	cube2->SetVertexBuffer(cubeVBuf2);
	cube2->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	cube2->SetMaterial(mat1);
	scene->AddObject(cube2);
	ge.AddObject("cube 2", cube2);
	#pragma endregion

	#pragma region SetUpPlane
	auto planeVBuf = Generate::PlaneXZWithNormals(100, 100, { 1, 1, 1, 1}, { 8, 8 });
	auto floorTex = std::make_shared<Texture>();
	floorTex->LoadTextureDataFromFile("floor.png");
	planeVBuf->SetTexture(floorTex);
	auto plane = std::make_shared<GraphicsObject>();
	plane->SetVertexBuffer(planeVBuf);
	plane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	plane->SetMaterial(mat1);
	scene->AddObject(plane);
	ge.AddObject("plane", plane);
	#pragma endregion

	#pragma region SetUpLocalLight
	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 1.0f);
	scene->SetLocalLight(localLight);
	#pragma endregion

	#pragma region SetUpGlobalLight
	auto globalLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto globalLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 1.0f);
	scene->SetGlobalLight(globalLight);
	#pragma endregion
}

static void SetUp3DScene3(std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene, GraphicsEnvironment& ge) {

	scene = std::make_shared<Scene>();

	#pragma region SetUpLightShader
	TextFile lightShadVertText;
	lightShadVertText.ReadAllLines("lighting.vert.glsl");
	TextFile lightShadFragText;
	lightShadFragText.ReadAllLines("lighting.frag.glsl");
	auto lightShad = std::make_shared<Shader>(lightShadVertText.GetContents(), lightShadFragText.GetContents());
	lightShad->AddUniform("world");
	lightShad->AddUniform("view");
	lightShad->AddUniform("projection");
	lightShad->AddUniform("materialAmbientIntensity");
	lightShad->AddUniform("materialSpecularIntensity");
	lightShad->AddUniform("materialShininess");
	lightShad->AddUniform("globalLightPosition");
	lightShad->AddUniform("globalLightColor");
	lightShad->AddUniform("globalLightIntensity");
	lightShad->AddUniform("localLightPosition");
	lightShad->AddUniform("localLightColor");
	lightShad->AddUniform("localLightIntensity");
	lightShad->AddUniform("localLightAttenuationCoef");
	lightShad->AddUniform("viewPosition");
	lightShad->AddUniform("tex");
	shader = lightShad;
	#pragma endregion

	#pragma region FallbackTexture
	auto dummyTex = std::make_shared<Texture>();
	dummyTex->LoadTextureDataFromFile("not-a-real-file.png");
	#pragma endregion

	auto mat1 = std::make_shared<Material>(1.0f, 1.0f, 1.0f);

	#pragma region SetUpCube1
	auto cubeVBuf1 = Generate::CuboidWithNormals(6, 2, 5);
	cubeVBuf1->SetTexture(dummyTex);
	auto cube1 = std::make_shared<GraphicsObject>();
	cube1->SetVertexBuffer(cubeVBuf1);
	cube1->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	cube1->SetMaterial(mat1);
	scene->AddObject(cube1);
	ge.AddObject("cube 1", cube1);

	auto rotateAnimation = std::make_shared<RotateAnimation>(glm::vec3{ 0.0f,1.0f,0.0f }, 90.0f);
	rotateAnimation->SetObject(cube1);
	cube1->SetAnimation(rotateAnimation);
	#pragma endregion

	#pragma region SetUpCube2
	auto crateTex = std::make_shared<Texture>();
	crateTex->LoadTextureDataFromFile("crate.png");
	auto cubeVBuf2 = Generate::CuboidWithNormals(10, 10, 10, { 1, 1, 1, 1 }, { 1, 1 });
	cubeVBuf2->SetTexture(crateTex);
	auto cube2 = std::make_shared<GraphicsObject>();
	cube2->SetVertexBuffer(cubeVBuf2);
	cube2->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	cube2->SetMaterial(mat1);
	scene->AddObject(cube2);
	ge.AddObject("cube 2", cube2);
	#pragma endregion

	#pragma region SetUpPlane
	auto planeVBuf = Generate::PlaneXZWithNormals(100, 100, { 1, 1, 1, 1 }, { 8, 8 });
	auto floorTex = std::make_shared<Texture>();
	floorTex->LoadTextureDataFromFile("floor.png");
	planeVBuf->SetTexture(floorTex);
	auto plane = std::make_shared<GraphicsObject>();
	plane->SetVertexBuffer(planeVBuf);
	plane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	plane->SetMaterial(mat1);
	scene->AddObject(plane);
	ge.AddObject("plane", plane);
	#pragma endregion

	#pragma region SetUpLocalLight
	auto localLightPos = glm::vec3{ 0, 10.0f, 0 };
	auto localLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto localLight = std::make_shared<Light>(localLightPos, localLightColor, 1.0f, 1.0f);
	scene->SetLocalLight(localLight);
	#pragma endregion

	#pragma region SetUpGlobalLight
	auto globalLightPos = glm::vec3{ 20.0f, 20.0f, 20.0f };
	auto globalLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };
	auto globalLight = std::make_shared<Light>(globalLightPos, globalLightColor, 1.0f, 1.0f);
	scene->SetGlobalLight(globalLight);

	#pragma region Sprite
	auto spriteVBuf = Generate::PlaneXYWithNormals(1, 1, { 1, 1, 1, 1 }, { 1, 1 });
	auto spriteTex = std::make_shared<Texture>();
	spriteTex->LoadTextureDataFromFile("lightbulb.png");
	spriteVBuf->SetTexture(spriteTex);
	auto sprite = std::make_shared<GraphicsObject>();
	sprite->SetVertexBuffer(spriteVBuf);
	sprite->SetPosition(localLightPos);
	scene->AddObject(sprite);
	ge.AddObject("sprite", sprite);
	#pragma endregion
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

	std::shared_ptr<Shader> shad3D;
	std::shared_ptr<Scene> scene3D;
	SetUp3DScene3(shad3D, scene3D, ge);

	ge.AddRenderer("3D renderer", shad3D, scene3D);
	ge.StaticAllocate();

	auto cam = std::make_shared<Camera>();
	ge.SetCamera(cam);

	ge.Run3D();

	return 0;
}
