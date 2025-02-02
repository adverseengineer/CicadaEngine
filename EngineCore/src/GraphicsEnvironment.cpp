#include "EventManager.h"
#include "GraphicsEnvironment.h"
#include "Logger.h"
#include "Shader.h"
#include "Timer.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Cicada;

MouseParams GraphicsEnvironment::mouse;

GraphicsEnvironment::GraphicsEnvironment() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); //adds antialiasing capabilities
}

GraphicsEnvironment::~GraphicsEnvironment() {
	glfwTerminate();
}

bool GraphicsEnvironment::SetWindow(unsigned int width, unsigned int height, const std::string& title) {
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {
		Logger::Write(LogEntry::Level::Error, "Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

bool GraphicsEnvironment::InitGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Logger::Write(LogEntry::Level::Error, "Failed to initialize GLAD");
		return false;
	}
	return true;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		Behavior::SetClickState(true);
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		Behavior::SetClickState(false);
}
void GraphicsEnvironment::SetupGraphics() {
	//set up a callback for whenever the window is resized
	glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
	glfwSetCursorPosCallback(window, OnMouseMove);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

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

static bool correctGamma = true;

void GraphicsEnvironment::ProcessInput(double elapsedSeconds) const {

	//if the user hits escape, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam->MoveZ((float)elapsedSeconds);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam->MoveZ((float)elapsedSeconds, -1);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam->MoveX((float)elapsedSeconds);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam->MoveX((float)elapsedSeconds, -1);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cam->MoveY((float)elapsedSeconds);
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cam->MoveY((float)elapsedSeconds, -1);
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

Ray mouseRay;

void GraphicsEnvironment::Run3D(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader) {

	glm::vec3 clearColor = { 0.1f, 0.1f, 0.1f };

	ImGuiIO& io = ImGui::GetIO();
	Timer timer;

	/*HighlightParams hlp = {
		&mouseRay
	};
	RotationParams rap = {
		&mouseRay,
		glm::vec3{0,1,0},
		45.0f
	};
	TranslationParams tap = {
		&mouseRay,
		glm::vec3{ 40,10,40 },
		glm::vec3{ 40,10,20 },
		0.0f,
		1,
		0.3f
	};*/

	auto dummy = ObjectManager::GetObject("dummy");
	//auto& dummyMat = dummy->GetMaterial();
	auto crate = ObjectManager::GetObject("crate");
	//auto& crateMat = crate->GetMaterial();
	auto mover = ObjectManager::GetObject("mover");
	//auto& moverMat = mover->GetMaterial();

	/*dummy->SetBehaviorParameters("highlight", hlp);
	crate->SetBehaviorParameters("highlight", hlp);
	mover->SetBehaviorParameters("highlight", hlp);
	mover->SetBehaviorParameters("translate", tap);
	mover->SetBehaviorParameters("rotate", rap);*/

	while (!glfwWindowShouldClose(window)) {
		double deltaTime = timer.GetElapsedTimeInSeconds();

		EventManager::TriggerEvent("OnUpdate");

		ProcessInput(deltaTime);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		mouse.windowWidth = windowWidth;
		mouse.windowHeight = windowHeight;

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		auto spher = mouse.spherical.ToMat4();
		auto pos = cam->GetPosition();
		cam->SetLocalTransform(spher);
		cam->SetPosition(pos);

		if (correctGamma)
			glEnable(GL_FRAMEBUFFER_SRGB);
		else
			glDisable(GL_FRAMEBUFFER_SRGB);

		if (windowWidth >= windowHeight)
			cam->m_aspectRatio = windowWidth / (windowHeight * 1.0f);
		else
			cam->m_aspectRatio = windowHeight / (windowWidth * 1.0f);

		cam->Update();

		for (const auto& [_, shader] : ShaderManager::GetAll()) {
			auto view = glm::inverse(cam->GetLocalTransform());
			shader->SendUniform("view", view);
			shader->SendUniform("projection", cam->m_projection);
		}

		auto& localLight = scene->GetLocalLight();
		auto& globalLight = scene->GetGlobalLight();

		//always make the lightbulb face towards the camera
		auto sprite = ObjectManager::GetObject("lightbulb");
		sprite->RotateToFace(cam->GetPosition());
		sprite->SetPosition(scene->GetLocalLight()->position);

		//mouseRay = cam->GetMouseRay((float) mouse.windowX, (float) mouse.windowY);

		//ObjectManager::Update(deltaTime);

		//and finally call render
		Renderer::RenderScene(scene, shader, cam);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::StyleColorsClassic();
		ImGui::NewFrame();
		ImGui::Begin("Interactive Graphics");		
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		
		auto& camTrans = cam->GetLocalTransform();
		ImGui::Text(
			"[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]",
			camTrans[0][0], camTrans[1][0], camTrans[2][0], camTrans[3][0],
			camTrans[0][1], camTrans[1][1], camTrans[2][1], camTrans[3][1],
			camTrans[0][2], camTrans[1][2], camTrans[2][2], camTrans[3][2],
			camTrans[0][3], camTrans[1][3], camTrans[2][3], camTrans[3][3]
		);

		ImGui::Checkbox("Correct Gamma", &correctGamma);

		ImGui::ColorEdit3("Background Color", glm::value_ptr(clearColor));

		ImGui::ColorEdit3("Local Light Color", glm::value_ptr(localLight->color));
		ImGui::DragFloat3("Local Light Position", glm::value_ptr(localLight->position));
		ImGui::SliderFloat("Local Intensity", &localLight->intensity, 0, 1);
		ImGui::SliderFloat("Local Attenuation", &localLight->attenuationCoef, 0, 1);
		
		ImGui::ColorEdit3("Global Light Color", glm::value_ptr(globalLight->color));
		ImGui::DragFloat3("Global Light Position", glm::value_ptr(globalLight->position));
		ImGui::SliderFloat("Global Intensity", &globalLight->intensity, 0, 1);
		ImGui::SliderFloat("Global Attenuation", &globalLight->attenuationCoef, 0, 1);

		ImGui::End();

		Logger::RenderLog();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
