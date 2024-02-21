#include "GraphicsEnvironment.h"
#include "Shader.h"

void GraphicsEnvironment::Init(unsigned int majorVersion, unsigned int minorVersion) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool GraphicsEnvironment::SetWindow(unsigned int width, unsigned int height, const std::string& title) {
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {
		Log("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	Log("Created GLFW window");
	return true;
}

bool GraphicsEnvironment::InitGlad(void) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Log("Failed to initialize GLAD");
		return false;
	}
	Log("Initialized GLAD");
	return true;
}

void GraphicsEnvironment::SetupGraphics(void) {
	//set up a callback for whenever the window is resized
	glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
	//set up ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	//enable transparency in textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	for(auto& kv : rendererMap)
		kv.second->StaticAllocateVertexBuffer();
}

void GraphicsEnvironment::Render(void) const {
	for(auto& kv : rendererMap)
		kv.second->RenderScene();
}