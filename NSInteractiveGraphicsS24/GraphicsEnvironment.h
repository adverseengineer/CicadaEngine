#pragma once
#include "Camera.h"
#include "GraphicsStructures.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Util.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

class GraphicsEnvironment {

protected:
	GLFWwindow* window;

	int windowWidth, windowHeight;

	ObjectManager objManager;
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;
	std::shared_ptr<Camera> cam;

	static MouseParams mouse;

public:
	inline GraphicsEnvironment() : window(nullptr), windowWidth(0), windowHeight(0) {}
	~GraphicsEnvironment();

	inline GLFWwindow* GetWindow() { return window; }

	void Init(unsigned int majorVersion, unsigned int minorVersion);
	bool SetWindow(unsigned int width, unsigned int height, const std::string& title);
	bool InitGlad();
	void SetupGraphics();
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);

	inline void AddRenderer(const std::string& name, std::shared_ptr<Shader> shad, std::shared_ptr<Scene> scene) {
		rendererMap.emplace(name, std::make_shared<Renderer>(shad, scene));
	}
	inline std::shared_ptr<Renderer> GetRenderer(const std::string& name) const {
		//don't worry about not found exceptions, its better than returning null from this. change over to std::optional eventually
		return rendererMap.at(name);
	}

	inline const std::shared_ptr<Camera>& GetCamera() const { return cam; }
	inline void SetCamera(const std::shared_ptr<Camera>& cam) { this->cam = cam; }

	void StaticAllocate() const;
	void Render() const;

	void ProcessInput(double elapsedSeconds) const;

	inline void AddObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj) { objManager.SetObject(key, obj); }
	inline std::shared_ptr<GraphicsObject> GetObject(const std::string& key) const { return objManager.GetObject(key); }

	static void OnMouseMove(GLFWwindow* window, double mouseX, double mouseY);

	void Run3D();
};
