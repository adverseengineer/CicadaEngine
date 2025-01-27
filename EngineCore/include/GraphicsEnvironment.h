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

protected:
	GLFWwindow* window = nullptr;

	int windowWidth = 0, windowHeight = 0;

	std::shared_ptr<Camera> cam;

	static MouseParams mouse;

public:
	GraphicsEnvironment();
	~GraphicsEnvironment();

	inline GLFWwindow* GetWindow() { return window; }

	bool SetWindow(unsigned int width, unsigned int height, const std::string& title);
	bool InitGlad();
	void SetupGraphics();
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);

	inline const std::shared_ptr<Camera>& GetCamera() const { return cam; }
	inline void SetCamera(const std::shared_ptr<Camera>& cam) { this->cam = cam; }

	void ProcessInput(double elapsedSeconds) const;

	inline void AddObject(const std::string& key, const std::shared_ptr<GameObject>& obj) { ObjectManager::AddObject(key, obj); }
	inline std::shared_ptr<GameObject> GetObject(const std::string& key) const { return ObjectManager::GetObject(key); }

	static void OnMouseMove(GLFWwindow* window, double mouseX, double mouseY);

	void Run3D(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader);
};

}