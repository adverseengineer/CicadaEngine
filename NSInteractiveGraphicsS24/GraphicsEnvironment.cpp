//#include "Behavior.h"
#include "GraphicsEnvironment.h"
#include "Shader.h"
#include "Timer.h"
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

MouseParams GraphicsEnvironment::mouse;

GraphicsEnvironment::~GraphicsEnvironment() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

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
		renderer->StaticAllocateBuffers();
}

void GraphicsEnvironment::Render() const {
	for(auto& [name, renderer] : rendererMap)
		renderer->RenderScene(cam);
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

void GraphicsEnvironment::Run3D() {

	glm::vec3 clearColor = { 0.04f, 0.19f, 0.19f };

	ImGuiIO& io = ImGui::GetIO();
	Timer timer;

	auto& sphere = objManager.GetObject("sphere");
	auto& sphereMat = sphere->GetMaterial();

	while (!glfwWindowShouldClose(window)) {
		double deltaTime = timer.GetElapsedTimeInSeconds();

		ProcessInput(deltaTime);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		mouse.windowWidth = windowWidth;
		mouse.windowHeight = windowHeight;

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		auto spher = mouse.spherical.ToMat4();
		cam->SetLookFrame(spher);

		if (correctGamma)
			glEnable(GL_FRAMEBUFFER_SRGB);
		else
			glDisable(GL_FRAMEBUFFER_SRGB);

		if (windowWidth >= windowHeight)
			cam->aspectRatio = windowWidth / (windowHeight * 1.0f);
		else
			cam->aspectRatio = windowHeight / (windowWidth * 1.0f);

		//update the view matrix for each renderer, and send the view and projection to the shader
		for (const auto& [name, renderer] : rendererMap) {
			auto view = cam->GetView();
			renderer->SetView(view);
			auto& shader = renderer->GetShader();
			shader->SendMat4Uniform("view", view);
			shader->SendMat4Uniform("projection", cam->projection);
		}

		auto& basicScene = GetRenderer("basic")->GetScene();
		auto& flatScene = GetRenderer("flat")->GetScene();
		auto& litScene = GetRenderer("lit")->GetScene();
		auto& localLight = litScene->GetLocalLight();
		auto& globalLight = litScene->GetGlobalLight();

		auto axis = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 temp = sphere->GetLocalReferenceFrame();
		temp = glm::rotate(temp, (float)glm::radians(10.0f * deltaTime), axis);
		sphere->SetLocalReferenceFrame(temp);
		
		mouseRay = cam->GetMouseRay((float) mouse.windowX, (float) mouse.windowY);

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

		ImGui::Text(
			"Mouse Ray: {(%.3f, %.3f, %.3f), (%.3f, %.3f, %.3f)}",
			mouseRay.origin.x, mouseRay.origin.y, mouseRay.origin.z,
			mouseRay.direction.x, mouseRay.direction.y, mouseRay.direction.z
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

		ImGui::Text(Util::GetLog().c_str());

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
