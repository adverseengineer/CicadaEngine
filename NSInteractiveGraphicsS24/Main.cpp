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

static void SetUpPlainScene(std::shared_ptr<Shader>& shadPtr, std::shared_ptr<Scene>& scenePtr) {
	
	scenePtr = std::make_shared<Scene>();

	#pragma region SetUpShader
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("basic.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("basic.frag.glsl");
	shadPtr = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	shadPtr->AddUniform("projection");
	shadPtr->AddUniform("world");
	shadPtr->AddUniform("view");
	#pragma endregion

	#pragma region square
	std::shared_ptr<GraphicsObject> square = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(6);
	buffer->AddVertexData(6, -5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, -5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, -5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexData(6, 5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexData(6, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexAttribute("position", 0, 3);
	buffer->AddVertexAttribute("color", 1, 3, 3);
	square->SetVertexBuffer(buffer);
	scenePtr->AddObject(square);
	#pragma endregion

	#pragma region triangle
	std::shared_ptr<GraphicsObject> triangle = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer2 = std::make_shared<VertexBuffer>(6);
	buffer2->AddVertexData(6, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(6, -5.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(6, 5.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexAttribute("position", 0, 3);
	buffer2->AddVertexAttribute("color", 1, 3, 3);
	triangle->SetVertexBuffer(buffer2);
	triangle->SetPosition(glm::vec3(30.0f, 0.0f, 0.0f));
	scenePtr->AddObject(triangle);
	#pragma endregion

	#pragma region line
	std::shared_ptr<GraphicsObject> line = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer3 = std::make_shared<VertexBuffer>(6);
	buffer3->SetPrimitiveType(GL_LINES);
	buffer3->AddVertexData(6, 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer3->AddVertexData(6, 0.0f, -2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer3->AddVertexAttribute("position", 0, 3);
	buffer3->AddVertexAttribute("color", 1, 3, 3);
	line->SetVertexBuffer(buffer3);
	line->SetPosition(glm::vec3(5.0f, -10.0f, 0.0f));
	triangle->AddChild(line);
	#pragma endregion
}

static void SetUpTexturedScene(std::shared_ptr<Shader>& shadPtr, std::shared_ptr<Scene>& scenePtr) {
	
	scenePtr = std::make_shared<Scene>();

	#pragma region SetUpShader
	//set up the shader that will be used for textured objects
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("texture.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("texture.frag.glsl");
	shadPtr = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	shadPtr->AddUniform("projection");
	shadPtr->AddUniform("world");
	shadPtr->AddUniform("view");
	shadPtr->AddUniform("texUnit");
	#pragma endregion

	#pragma region SetUpTexture1
	std::shared_ptr<Texture> texPtr1 = std::make_shared<Texture>();
	texPtr1->SetDimensions(4, 4);
	unsigned char rawTextureData[64] = {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	texPtr1->SetTextureData(64, rawTextureData);
	texPtr1->SetWrapS(GL_CLAMP_TO_EDGE);
	texPtr1->SetWrapT(GL_CLAMP_TO_EDGE);
	texPtr1->SetMagFilter(GL_LINEAR);
	#pragma endregion

	#pragma region SetUpTexture2
	std::shared_ptr<Texture> texPtr2 = std::make_shared<Texture>();
	texPtr2->LoadTextureDataFromFile("gw.png");
	//TODO: set wrap and filters for this texture?
	#pragma endregion

	#pragma region TexturedObject1
	std::shared_ptr<GraphicsObject> objPtr1 = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> vertBufPtr1 = std::make_shared<VertexBuffer>(8);
	//add vertices and uv's to the vertex buffer
	vertBufPtr1->AddVertexData(8, -20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f);
	vertBufPtr1->AddVertexData(8, -20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vertBufPtr1->AddVertexData(8, 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f);
	vertBufPtr1->AddVertexData(8, -20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f);
	vertBufPtr1->AddVertexData(8, 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f);
	vertBufPtr1->AddVertexData(8, 20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 3.0f);
	//add vertex attributes to the vertex buffer
	vertBufPtr1->AddVertexAttribute("position", 0, 3, 0);
	vertBufPtr1->AddVertexAttribute("vertexColor", 1, 3, 3);
	vertBufPtr1->AddVertexAttribute("texCoord", 2, 2, 6);

	vertBufPtr1->SetTexturePtr(texPtr1); //assign the texture to the vertex buffer
	objPtr1->SetVertexBuffer(vertBufPtr1); //assign the vertex data to the object
	objPtr1->SetPosition(glm::vec3(-30.0f, -20.0f, 0.0f)); //give the object a location
	scenePtr->AddObject(objPtr1); //and add it into the scene
	#pragma endregion

	#pragma region TexturedObject2
	std::shared_ptr<GraphicsObject> objPtr2 = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> vertBufPtr2 = std::make_shared<VertexBuffer>(8);

	//add vertices and uv's to the vertex buffer
	vertBufPtr2->AddVertexData(8, -20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vertBufPtr2->AddVertexData(8, -20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vertBufPtr2->AddVertexData(8, 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vertBufPtr2->AddVertexData(8, -20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vertBufPtr2->AddVertexData(8, 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vertBufPtr2->AddVertexData(8, 20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//add vertex attributes to the vertex buffer
	vertBufPtr2->AddVertexAttribute("position", 0, 3, 0);
	vertBufPtr2->AddVertexAttribute("vertexColor", 1, 3, 3);
	vertBufPtr2->AddVertexAttribute("texCoord", 2, 2, 6);

	vertBufPtr2->SetTexturePtr(texPtr2); //assign the texture to the vertex buffer
	objPtr2->SetVertexBuffer(vertBufPtr2); //assign the vertex data to the object
	objPtr2->SetPosition(glm::vec3(30.0f, 20.0f, 0.0f)); //give the object a location
	scenePtr->AddObject(objPtr2); //and add it into the scene
	#pragma endregion
}

static void SetUp3DScene1(std::shared_ptr<Shader>& shadPtr, std::shared_ptr<Scene>& scenePtr) {

	scenePtr = std::make_shared<Scene>();

	#pragma region SetUpShader
	//set up the shader that will be used for textured objects
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("texture.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("texture.frag.glsl");
	shadPtr = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	shadPtr->AddUniform("projection");
	shadPtr->AddUniform("world");
	shadPtr->AddUniform("view");
	shadPtr->AddUniform("texUnit");
	#pragma endregion

	#pragma region RawTexture
	auto rawTexPtr = std::make_shared<Texture>();
	rawTexPtr->SetDimensions(2, 2);
	unsigned char rawTextureData[16] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
	rawTexPtr->SetTextureData(16, rawTextureData);
	rawTexPtr->SetWrapS(GL_REPEAT);
	rawTexPtr->SetWrapT(GL_REPEAT);
	rawTexPtr->SetMagFilter(GL_NEAREST);
	#pragma endregion

	#pragma region SetUpCube1
	auto cubeVBufPtr1 = Generate::Cuboid(10, 10, 10, {0,0,0},{1,1});
	cubeVBufPtr1->SetTexturePtr(rawTexPtr);
	auto cubePtr1 = std::make_shared<GraphicsObject>();
	cubePtr1->SetVertexBuffer(cubeVBufPtr1);
	cubePtr1->SetPosition(glm::vec3(-10.0f, 10.0f, 0.0f));
	scenePtr->AddObject(cubePtr1);
	#pragma endregion

	#pragma region SetUpCube2
	auto crateTexPtr = std::make_shared<Texture>();
	crateTexPtr->LoadTextureDataFromFile("crate.png");
	auto cubeVBufPtr2 = Generate::Cuboid(1, 2, 3);
	cubeVBufPtr2->SetTexturePtr(crateTexPtr);
	auto cubePtr2 = std::make_shared<GraphicsObject>();
	cubePtr2->SetVertexBuffer(cubeVBufPtr2);
	cubePtr2->SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	scenePtr->AddObject(cubePtr2);
	#pragma endregion

	#pragma region SetUpPlane
	auto planeVBufPtr = Generate::Plane(200, 200, {0,0,0}, {8,8});
	auto floorTexPtr = std::make_shared<Texture>();
	floorTexPtr->LoadTextureDataFromFile("floor.png");
	planeVBufPtr->SetTexturePtr(floorTexPtr);
	auto planePtr = std::make_shared<GraphicsObject>();
	planePtr->SetVertexBuffer(planeVBufPtr);
	planePtr->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	scenePtr->AddObject(planePtr);
	#pragma endregion
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	GraphicsEnvironment glfw;
	glfw.Init(4, 3);
	bool created = glfw.SetWindow(1200, 800, "ETSU Computing Interactive Graphics");
	if (created == false)
		return -1;
	bool loaded = glfw.InitGlad();
	if (loaded == false)
		return -1;

	glfw.SetupGraphics();

	std::shared_ptr<Shader> threeDShadPtr;
	std::shared_ptr<Scene> threeDScenePtr;
	SetUp3DScene1(threeDShadPtr, threeDScenePtr);

	glfw.CreateRenderer("3D renderer", threeDShadPtr, threeDScenePtr);
	glfw.StaticAllocate();

	glfw.Run3D();

	return 0;
}
