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

static void SetUpTexturedScene(std::shared_ptr<Shader>& texShadPtr, std::shared_ptr<Scene>& texScenePtr) {
	
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

	#pragma region SetUpTexture2
	std::shared_ptr<Texture> texPtr2 = std::make_shared<Texture>();
	texPtr2->LoadTextureDataFromFile("gw.png");
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

void OnWindowSizeChanged(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {

	glm::vec3 right = glm::cross(direction, up);
	right = glm::normalize(right);

	glm::vec3 vUp = glm::cross(right, direction);
	vUp = glm::normalize(vUp);

	glm::mat4 view(1.0f);
	view[0] = glm::vec4(right, 0.0f);
	view[1] = glm::vec4(up, 0.0f);
	view[2] = glm::vec4(direction, 0.0f);
	view[3] = glm::vec4(position, 1.0f);
	return glm::inverse(view);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {

	GraphicsEnvironment glfw;
	glfw.Init(4, 3);
	bool created = glfw.SetWindow(
		1200, 800, "ETSU Computing Interactive Graphics");
	if (created == false) return -1;
	bool loaded = glfw.InitGlad();
	if (loaded == false) return -1;
	glfw.SetupGraphics();
	GLFWwindow* window = glfw.GetWindow();
;
	TextFile vertShadFile;
	vertShadFile.ReadAllLines("basic.vert.glsl");
	TextFile fragShadFile;
	fragShadFile.ReadAllLines("basic.frag.glsl");

	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertShadFile.GetContents(), fragShadFile.GetContents());
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	unsigned int shaderProgram = shader->GetShaderProgram();

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspectRatio = width / (height * 1.0f);

	float left = -50.0f;
	float right = 50.0f;
	float bottom = -50.0f;
	float top = 50.0f;
	left *= aspectRatio;
	right *= aspectRatio;
	glm::mat4 projection;

	std::shared_ptr<Scene> scenePtr = std::make_shared<Scene>();
	Renderer rend(shader, scenePtr);

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

	rend.StaticAllocateVertexBuffer();

	std::shared_ptr<Shader> texShadPtr;
	std::shared_ptr<Scene> texScenePtr = std::make_shared<Scene>();
	SetUpTexturedScene(texShadPtr, texScenePtr);
	Renderer texRend = Renderer(texShadPtr, texScenePtr);

	texRend.StaticAllocateVertexBuffer();

	glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

	glUseProgram(shaderProgram);

	float angle = 0, childAngle = 0;
	float cameraX = -10, cameraY = 0;
	glm::mat4 view;

	ImGuiIO& io = ImGui::GetIO();
	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		glfwGetWindowSize(window, &width, &height);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		view = CreateViewMatrix(
			glm::vec3(cameraX, cameraY, 1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		shader->SendMat4Uniform("projection", projection);
		texShadPtr->SendMat4Uniform("projection", projection);

		if (width >= height) {
			aspectRatio = width / (height * 1.0f);
		}
		else {
			aspectRatio = height / (width * 1.0f);
		}

		texRend.SetView(view);
		rend.SetView(view);

		//update the objects in the scene
		for (auto& object : scenePtr->GetObjects()) {
			object->ResetOrientation();
			object->RotateLocalZ(angle);
			for (auto& child : object->GetChildren()) {
				child->ResetOrientation();
				child->RotateLocalZ(childAngle);
			}
		}

		texRend.RenderScene();
		rend.RenderScene();

		#pragma region ImGui Update
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		ImGui::Text(shader->GetLog().c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("Angle", &angle, 0, 360);
		ImGui::SliderFloat("Child Angle", &childAngle, 0, 360);
		ImGui::SliderFloat("Camera X", &cameraX, left, right);
		ImGui::SliderFloat("Camera Y", &cameraY, bottom, top);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
