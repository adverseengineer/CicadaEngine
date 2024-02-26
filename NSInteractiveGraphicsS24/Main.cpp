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

static void SetUpTexturedScene(std::shared_ptr<Shader>& texShadPtr, std::shared_ptr<Scene>& texScenePtr) {
	
	texScenePtr = std::make_shared<Scene>();

	#pragma region SetUpShader
	//set up the shader that will be used for textured objects
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("texture.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("texture.frag.glsl");
	texShadPtr = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	texShadPtr->AddUniform("projection");
	texShadPtr->AddUniform("world");
	texShadPtr->AddUniform("view");
	texShadPtr->AddUniform("texUnit");
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
	texScenePtr->AddObject(objPtr1); //and add it into the scene
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
	texScenePtr->AddObject(objPtr2); //and add it into the scene
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

	std::shared_ptr<Shader> plainShadPtr;
	std::shared_ptr<Scene> plainScenePtr;
	SetUpPlainScene(plainShadPtr, plainScenePtr);

	std::shared_ptr<Shader> texShadPtr;
	std::shared_ptr<Scene> texScenePtr;
	SetUpTexturedScene(texShadPtr, texScenePtr);

	glfw.CreateRenderer("plain renderer", plainShadPtr, plainScenePtr);
	glfw.CreateRenderer("textured renderer", texShadPtr, texScenePtr);
	glfw.StaticAllocate();

	glfw.Run2D();

	return 0;
}
