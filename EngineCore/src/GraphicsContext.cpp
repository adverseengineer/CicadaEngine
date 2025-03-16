
#include "GraphicsContext.h"
#include <stdexcept>
#include <glad/glad.h>

using namespace Cicada;

bool GraphicsContext::gladInitialized = false;

GraphicsContext::GraphicsContext() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); //adds antialiasing capabilities
	//TODO: make this more sophisticated and configurable
}

GraphicsContext::~GraphicsContext() {
	glfwTerminate();
}

void GraphicsContext::CreateWindow(unsigned int width, unsigned int height, const std::string& title) {

	if (m_windowHandle != nullptr)
		throw std::runtime_error("window has already been initialized");

	if ((m_windowHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL)) == NULL) {
		glfwTerminate();
		throw std::runtime_error("failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_windowHandle); //attach the OpenGL context (state) to this window

	m_mouse.windowWidth = width;
	m_mouse.windowHeight = height;
}

void GraphicsContext::InitGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("failed to initialize GLAD");
	}
	GraphicsContext::gladInitialized = true;
}

void GraphicsContext::Configure() {

	if (!gladInitialized)
		throw std::runtime_error("must initialize GLAD first");

	//set up a callback for whenever the window is resized
	glfwSetFramebufferSizeCallback(m_windowHandle, OnWindowResize);
	glfwSetCursorPosCallback(m_windowHandle, OnMouseMove);
	glfwSetMouseButtonCallback(m_windowHandle, OnMouseButton);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

void GraphicsContext::RegisterCallbacks() {
	glfwSetFramebufferSizeCallback(m_windowHandle, OnWindowResize);
	glfwSetCursorPosCallback(m_windowHandle, OnMouseMove);
	glfwSetMouseButtonCallback(m_windowHandle, OnMouseButton);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //<<-- only enable if you don't want to click things
}

void GraphicsContext::OnWindowResize(GLFWwindow* window, int width, int height) {
	static auto& context = Instance();
	//TODO: ^^^ feels very icky to get a singleton reference within a static callback, possibly rework?
	//but also, how often is this particular callback gonna fire anyways? maybe its fine?
	context.SetWidth(width);
	context.SetHeight(height);
	context.m_mouse.windowX = width; //also set the screen size for the mouse. note: this sucks. its only temporary
	context.m_mouse.windowY = height;
	glViewport(0, 0, width, height);
}

void GraphicsContext::OnMouseMove(GLFWwindow* window, double mouseX, double mouseY) {

	static auto& context = Instance(); //TODO: okay singleton reference here also sucks. find a better way
	
	context.m_mouse.x = mouseX;
	context.m_mouse.y = mouseY;
}

void GraphicsContext::OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
	//TODO: nothing to do here just yet
}
