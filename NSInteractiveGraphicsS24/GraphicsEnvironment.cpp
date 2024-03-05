#include "GraphicsEnvironment.h"
#include "Shader.h"
#include "Timer.h"
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include "RotateAnimation.h"

GraphicsEnvironment* GraphicsEnvironment::self;

void GraphicsEnvironment::Init(unsigned int majorVersion, unsigned int minorVersion) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); //adds antialiasing capabilities
}

bool GraphicsEnvironment::SetWindow(unsigned int width, unsigned int height, const std::string& title) {
	winPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (winPtr == NULL) {
		Util::Log("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(winPtr);
	Util::Log("Created GLFW window");
	return true;
}

bool GraphicsEnvironment::InitGlad(void) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Util::Log("Failed to initialize GLAD");
		return false;
	}
	Util::Log("Initialized GLAD");
	return true;
}

void GraphicsEnvironment::SetupGraphics(void) {
	//set up a callback for whenever the window is resized
	glfwSetFramebufferSizeCallback(winPtr, OnWindowSizeChanged);
	glfwSetCursorPosCallback(winPtr, OnMouseMove);
	glfwSetInputMode(winPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set up ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(winPtr, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	//enable transparency in textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//enable culling of backfaces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//enable antialiasing
	glEnable(GL_MULTISAMPLE);
}

void GraphicsEnvironment::OnWindowSizeChanged(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void GraphicsEnvironment::CreateRenderer(const std::string& name, std::shared_ptr<Shader> shadPtr, std::shared_ptr<Scene> scenePtr) {
	rendererMap.emplace(name, std::make_shared<Renderer>(shadPtr, scenePtr));
}

std::shared_ptr<Renderer> GraphicsEnvironment::GetRenderer(const std::string& name) const {
	//don't worry about not found exceptions, its better than returning null from this. change over to std::optional eventually
	return rendererMap.at(name);
}

void GraphicsEnvironment::StaticAllocate(void) const {
	for(auto& pair : rendererMap)
		pair.second->StaticAllocateVertexBuffer();
}

void GraphicsEnvironment::Render(void) const {
	for(auto& pair : rendererMap)
		pair.second->RenderScene();
}

void GraphicsEnvironment::ProcessInput(float elapsedSeconds) const {

	//if the user hits escape, close the window
	if (glfwGetKey(winPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(winPtr, true);

	if (glfwGetKey(winPtr, GLFW_KEY_W) == GLFW_PRESS)
		cam->MoveZ(elapsedSeconds);
	else if (glfwGetKey(winPtr, GLFW_KEY_S) == GLFW_PRESS)
		cam->MoveZ(elapsedSeconds, -1);

	if (glfwGetKey(winPtr, GLFW_KEY_A) == GLFW_PRESS)
		cam->MoveX(elapsedSeconds);
	else if (glfwGetKey(winPtr, GLFW_KEY_D) == GLFW_PRESS)
		cam->MoveX(elapsedSeconds, -1);

	if (glfwGetKey(winPtr, GLFW_KEY_Q) == GLFW_PRESS)
		cam->MoveY(elapsedSeconds);
	else if (glfwGetKey(winPtr, GLFW_KEY_E) == GLFW_PRESS)
		cam->MoveY(elapsedSeconds, -1);
}

glm::mat4 GraphicsEnvironment::CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {

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

void GraphicsEnvironment::AddObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj) {
	objManager.SetObject(key, obj);
}

std::shared_ptr<GraphicsObject> GraphicsEnvironment::GetObject(const std::string& key) const {
	return objManager.GetObject(key);
}

void GraphicsEnvironment::OnMouseMove(GLFWwindow* window, double mouseX, double mouseY) {

	self->mouse.x = mouseX;
	self->mouse.y = mouseY;

	float xPercent = static_cast<float>(self->mouse.x / self->mouse.windowWidth);
	float yPercent = static_cast<float>(self->mouse.y / self->mouse.windowHeight);

	self->mouse.spherical.theta = 90.0f - (xPercent * 180); // left/right
	self->mouse.spherical.phi = 180.0f - (yPercent * 180); // up/down
}

void GraphicsEnvironment::Run2D(void) {

	float camX = -10, camY = 0;

	int width, height;
	glfwGetWindowSize(winPtr, &width, &height);
	float aspectRatio = width / (height * 1.0f);

	float left = -50.0f;
	float right = 50.0f;
	float bottom = -50.0f;
	float top = 50.0f;
	left *= aspectRatio;
	right *= aspectRatio;

	glm::mat4 view, projection;
	glm::vec3 clearColor = { 0.1f, 0.9f, 0.2f };
	
	float angle = 3.141592654f, childAngle = 22.0f;

	ImGuiIO& io = ImGui::GetIO();
	while (!glfwWindowShouldClose(winPtr)) {

		ProcessInput(0);

		//if the window size has changed, recalculate the aspect ratio 
		glfwGetWindowSize(winPtr, &width, &height);
		if (width >= height)
			aspectRatio = width / (height * 1.0f);
		else
			aspectRatio = height / (width * 1.0f);

		//clear the screen
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		view = GraphicsEnvironment::CreateViewMatrix(
			glm::vec3(camX, camY, 1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		//update the view matrix for each renderer, and send the view and projection to the shader
		for (auto& pair : rendererMap) {
			pair.second->SetView(view);
			pair.second->GetShader()->SendMat4Uniform("view", view);
			pair.second->GetShader()->SendMat4Uniform("projection", projection);
		}

		//do the rotate thing for the plain untextured objects
		for (auto& object : GetRenderer("plain renderer")->GetScene()->GetObjects()) {
			object->ResetOrientation();
			object->RotateLocalZ(angle);
			for (auto& child : object->GetChildren()) {
				child->ResetOrientation();
				child->RotateLocalZ(childAngle);
			}
		}

		//update each textured object
		for (auto& object : GetRenderer("textured renderer")->GetScene()->GetObjects()) {
			//TODO: something to each of the textured objects
		}

		//let each renderer do their thing
		for (auto& pair : rendererMap) {
			pair.second->RenderScene();
		}

		#pragma region ImGui Update
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		ImGui::Text(Util::GetLog().c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("Angle", &angle, 0, 360);
		ImGui::SliderFloat("Child Angle", &childAngle, 0, 360);
		ImGui::SliderFloat("Camera X", &camX, left, right);
		ImGui::SliderFloat("Camera Y", &camY, bottom, top);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#pragma endregion

		glfwSwapBuffers(winPtr);
		glfwPollEvents();
	}
}

void GraphicsEnvironment::Run3D(void) {

	float camX = 0, camY = 0, camZ = 0;

	int windowWidth, windowHeight;
	glfwGetWindowSize(winPtr, &windowWidth, &windowHeight);
	float aspectRatio = windowWidth / (windowHeight * 1.0f);

	float left = -50.0f;
	float right = 50.0f;
	float bottom = -50.0f;
	float top = 50.0f;
	left *= aspectRatio;
	right *= aspectRatio;

	float fov = 60.0f;
	float nearClip = 0.01f;
	float farClip = 200.0f;

	glm::vec3 clearColor = { 0.66f, 0.66f, 0.66f };

	cam->SetPosition({ 0.0f, 15.0f, -30.0f });
	glm::mat4 model, view, projection;

	float cubeXAngle = 0, cubeYAngle = 0, cubeZAngle = 0;

	std::shared_ptr<RotateAnimation> rotateAnimation = std::make_shared<RotateAnimation>();
	rotateAnimation->SetObject(objManager.GetObject("cube 2"));
	objManager.GetObject("cube 2")->SetAnimation(rotateAnimation);

	ImGuiIO& io = ImGui::GetIO();
	Timer timer;
	while (!glfwWindowShouldClose(winPtr)) {
		double deltaTime = timer.GetElapsedTimeInSeconds();

		mouse.windowWidth = windowWidth;
		mouse.windowHeight = windowHeight;

		ProcessInput(deltaTime);
		glfwGetWindowSize(winPtr, &windowWidth, &windowHeight);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		model = glm::rotate(glm::mat4(1.0f), glm::radians(cubeYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(cubeXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(cubeZAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		cam->SetLookFrame(mouse.spherical.ToMat4());
		view = cam->LookForward();

		if (windowWidth >= windowHeight)
			aspectRatio = windowWidth / (windowHeight * 1.0f);
		else
			aspectRatio = windowHeight / (windowWidth * 1.0f);

		projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

		//update the view matrix for each renderer, and send the view and projection to the shader
		for (auto& pair : rendererMap) {
			pair.second->SetView(view);
			pair.second->GetShader()->SendMat4Uniform("view", view);
			pair.second->GetShader()->SendMat4Uniform("projection", projection);
		}

		objManager.GetObject("cube 1")->ResetOrientation();
		objManager.GetObject("cube 1")->RotateLocal(cubeXAngle, cubeYAngle, cubeZAngle);

		objManager.Update(deltaTime);

		//and finally call render
		for (auto& pair : rendererMap) {
			pair.second->RenderScene();
		}

		float camSpeed = 10;
		cam->SetMoveSpeed(camSpeed);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("Cube X Angle", &cubeXAngle, 0, 360);
		ImGui::SliderFloat("Cube Y Angle", &cubeYAngle, 0, 360);
		ImGui::SliderFloat("Cube Z Angle", &cubeZAngle, 0, 360);
		ImGui::SliderFloat("Camera Speed", &camSpeed, 0, 20);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text(Util::GetLog().c_str());
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(winPtr);
		glfwPollEvents();
	}
}