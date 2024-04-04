#include "GraphicsEnvironment.h"
#include "RotateAnimation.h"
#include "Shader.h"
#include "Timer.h"
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

MouseParams GraphicsEnvironment::mouse;

void GraphicsEnvironment::Init(unsigned int majorVersion, unsigned int minorVersion) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); //adds antialiasing capabilities
}

bool GraphicsEnvironment::SetWindow(unsigned int width, unsigned int height, const std::string& title) {
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {
		Util::Log("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	Util::Log("Created GLFW window");
	return true;
}

bool GraphicsEnvironment::InitGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Util::Log("Failed to initialize GLAD");
		return false;
	}
	Util::Log("Initialized GLAD");
	return true;
}

void GraphicsEnvironment::SetupGraphics() {
	//set up a callback for whenever the window is resized
	glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
	glfwSetCursorPosCallback(window, OnMouseMove);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set up ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	//enable transparency in textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//enable culling of backfaces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//enable antialiasing
	glEnable(GL_MULTISAMPLE);
}

void GraphicsEnvironment::OnWindowSizeChanged(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void GraphicsEnvironment::StaticAllocate(void) const {
	for(auto& [name, renderer] : rendererMap)
		renderer->StaticAllocateVertexBuffer();
}

void GraphicsEnvironment::Render() const {
	for(auto& [name, renderer] : rendererMap)
		renderer->RenderScene(cam);
}

static bool freeCamMode = true;
static bool correctGamma = true;

static bool spaceCurrent = false;
static bool spaceOld = false;

static bool on = false;
static bool onOld = false;

void GraphicsEnvironment::ProcessInput(float elapsedSeconds) const {

	//if the user hits escape, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(freeCamMode) {

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam->MoveZ(elapsedSeconds);
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam->MoveZ(elapsedSeconds, -1);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam->MoveX(elapsedSeconds);
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam->MoveX(elapsedSeconds, -1);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cam->MoveY(elapsedSeconds);
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cam->MoveY(elapsedSeconds, -1);
	}

	else {
		auto id = glm::mat4(1.0f);
		auto up = glm::vec3(0.0f, 1.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			cam->SetPosition({ 0.0f, 5.0f, 60.0f });
			cam->SetLookFrame(glm::rotate(id, glm::radians(0.0f), up));
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			cam->SetPosition({ 60.0f, 5.0f, 0.0f });
			cam->SetLookFrame(glm::rotate(id, glm::radians(90.0f), up));
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			cam->SetPosition({ 0.0f, 5.0f, -60.0f });
			cam->SetLookFrame(glm::rotate(id, glm::radians(180.0f), up));
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
			cam->SetPosition({ -60.0f, 5.0f, 0.0f });
			cam->SetLookFrame(glm::rotate(id, glm::radians(270.0f), up));
		}
	}
}

void GraphicsEnvironment::OnMouseMove(GLFWwindow* window, double mouseX, double mouseY) {

	auto& mouse = GraphicsEnvironment::mouse;

	mouse.x = mouseX;
	mouse.y = mouseY;

	float xPercent = static_cast<float>(mouse.x / mouse.windowWidth);
	float yPercent = static_cast<float>(mouse.y / mouse.windowHeight);

	mouse.spherical.theta = 90.0f - (xPercent * 180); //left/right
	mouse.spherical.phi = 180.0f - (yPercent * 180); //up/down
}

void GraphicsEnvironment::Run3D() {

	int windowWidth = 0, windowHeight = 0;
	float aspectRatio;

	float fov = 60.0f;
	float nearClip = 0.01f;
	float farClip = 200.0f;

	glm::vec3 clearColor = { 0.5f, 0.9f, 1.0f };

	cam->SetPosition({0.0f, 15.0f, 30.0f });
	glm::mat4 model(1), view(1), projection(1);

	ImGuiIO& io = ImGui::GetIO();
	Timer timer;

	while (!glfwWindowShouldClose(window)) {
		double deltaTime = timer.GetElapsedTimeInSeconds();

		ProcessInput(deltaTime);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		mouse.windowWidth = windowWidth;
		mouse.windowHeight = windowHeight;

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (freeCamMode) {
			auto spher = mouse.spherical.ToMat4();
			cam->SetLookFrame(spher);
		}
			
		view = cam->GetView();

		if (correctGamma)
			glEnable(GL_FRAMEBUFFER_SRGB);
		else
			glDisable(GL_FRAMEBUFFER_SRGB);

		if (windowWidth >= windowHeight)
			aspectRatio = windowWidth / (windowHeight * 1.0f);
		else
			aspectRatio = windowHeight / (windowWidth * 1.0f);

		projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

		//update the view matrix for each renderer, and send the view and projection to the shader
		for (const auto& [name, renderer] : rendererMap) {
			renderer->SetView(view);
			renderer->GetShader()->SendMat4Uniform("view", view);
			renderer->GetShader()->SendMat4Uniform("projection", projection);
		}

		auto& sprite = objManager.GetObject("lightbulb");
		auto& litScene = GetRenderer("lit renderer")->GetScene();
		sprite->RotateToFace(cam->GetPosition());
		sprite->SetPosition(litScene->GetLocalLight()->position);

		objManager.Update(deltaTime);

		//and finally call render
		for (const auto& [name, renderer] : rendererMap) {
			renderer->RenderScene(cam);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::StyleColorsClassic();
		ImGui::NewFrame();
		ImGui::Begin("Interactive Graphics");		
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		auto& camPos = cam->GetPosition();
		ImGui::Text("CamPos: (%.3f, %.3f, %.3f)", camPos.x, camPos.y, camPos.z);
		auto& camLook = cam->GetLookFrame();
		ImGui::Text(
			"[%.3f %.3f %.3f]\n[%.3f %.3f %.3f]\n[%.3f %.3f %.3f]",
			camLook[0][0], camLook[1][0], camLook[2][0],
			camLook[0][1], camLook[1][1], camLook[2][1],
			camLook[0][2], camLook[1][2], camLook[2][2]
		);

		ImGui::Checkbox("Free Cam", &freeCamMode);
		ImGui::Checkbox("Correct Gamma", &correctGamma);

		ImGui::ColorEdit3("Background Color", glm::value_ptr(clearColor));

		ImGui::ColorEdit3("Local Light Color", glm::value_ptr(litScene->GetLocalLight()->color));
		ImGui::DragFloat3("Local Light Position", glm::value_ptr(litScene->GetLocalLight()->position));
		ImGui::SliderFloat("Local Intensity", &litScene->GetLocalLight()->intensity, 0, 1);
		ImGui::SliderFloat("Local Attenuation", &litScene->GetLocalLight()->attenuationCoef, 0, 1);
		
		ImGui::ColorEdit3("Global Light Color", glm::value_ptr(litScene->GetGlobalLight()->color));
		ImGui::DragFloat3("Global Light Position", glm::value_ptr(litScene->GetGlobalLight()->position));
		ImGui::SliderFloat("Global Intensity", &litScene->GetGlobalLight()->intensity, 0, 1);
		ImGui::SliderFloat("Global Attenuation", &litScene->GetGlobalLight()->attenuationCoef, 0, 1);
		
		auto& mat = objManager.GetObject("crate")->GetMaterial();
		ImGui::SliderFloat("Material Ambient Intensity", (float*)&mat->ambientIntensity, 0, 1);
		ImGui::SliderFloat("Material Specular Intensity", (float*)&mat->specularIntensity, 0, 1);
		ImGui::SliderFloat("Material Shininess", (float*)&mat->shininess, 0, 100);

		ImGui::Text(Util::GetLog().c_str());

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
