#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "Generate.h"
#include "GraphicsEnvironment.h"
#include "GraphicsObject.h"
#include "Scene.h"

#include "Renderer.h"
#include "Shader.h"
#include "TextFile.h"

static void SetUp3DScene1(std::shared_ptr<Shader>& shad, std::shared_ptr<Scene>& scene) {

	scene = std::make_shared<Scene>();

	#pragma region SetUpShader
	//set up the shader that will be used for textured objects
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("texture.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("texture.frag.glsl");
	shad = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	shad->AddUniform("projection");
	shad->AddUniform("world");
	shad->AddUniform("view");
	shad->AddUniform("texUnit");
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

	#pragma region SetUpCube1
	auto cubeVBuf1 = Generate::Cuboid(10, 10, 10, {1,1,1},{1,1});
	cubeVBuf1->SetTexturePtr(rawTex);
	auto cube1 = std::make_shared<GraphicsObject>();
	cube1->SetVertexBuffer(cubeVBuf1);
	cube1->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	scene->AddObject(cube1);
	#pragma endregion

	#pragma region SetUpCube2
	auto crateTex = std::make_shared<Texture>();
	crateTex->LoadTextureDataFromFile("crate.png");
	auto cubeVBuf2 = Generate::Cuboid(1, 2, 3);
	cubeVBuf2->SetTexturePtr(crateTex);
	auto cube2 = std::make_shared<GraphicsObject>();
	cube2->SetVertexBuffer(cubeVBuf2);
	cube2->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	scene->AddObject(cube2);
	#pragma endregion

	#pragma region SetUpPlane
	auto planeVBuf = Generate::Plane(200, 200, {1,1,1}, {8,8});
	auto floorTex = std::make_shared<Texture>();
	floorTex->LoadTextureDataFromFile("floor.png");
	planeVBuf->SetTexturePtr(floorTex);
	auto plane = std::make_shared<GraphicsObject>();
	plane->SetVertexBuffer(planeVBuf);
	plane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	scene->AddObject(plane);
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
	SetUp3DScene1(shad3D, scene3D);

	ge.CreateRenderer("3D renderer", shad3D, scene3D);
	ge.StaticAllocate();

	ge.Run3D();

	return 0;
}
