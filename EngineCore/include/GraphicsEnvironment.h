#pragma once
#include "Camera.h"
#include "GraphicsStructures.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "Util.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace Cicada {

class GraphicsEnvironment {

private:
	GLFWwindow* m_windowHandle = nullptr;
	int m_windowWidth = 0, m_windowHeight = 0;

	std::shared_ptr<Camera> m_cam;
	static MouseParams s_mouse;

	GraphicsEnvironment();
	~GraphicsEnvironment();

public:
	inline static GraphicsEnvironment& Instance() {
		static GraphicsEnvironment instance;
		return instance;
	}

	inline GLFWwindow* GetWindow() { return m_windowHandle; }

	void CreateWindow(unsigned int width, unsigned int height, const std::string& title);
	void InitGlad();
	
	void SetupGraphics();
	
	inline const std::shared_ptr<Camera>& GetCamera() const { return m_cam; }
	inline void SetCamera(const std::shared_ptr<Camera>& cam) { this->m_cam = cam; }

	void ProcessInput(double elapsedSeconds) const;

	static void OnWindowSizeChanged(GLFWwindow* windowHandle, int width, int height);
	static void OnMouseMove(GLFWwindow* windowHandle, double mouseX, double mouseY);

	void Run3D(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader);
};

}