#pragma once
#include "BaseObject.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include "Scene.h"
#include "Renderer.h"

class GraphicsEnvironment : public BaseObject {

protected:
	GLFWwindow* winPtr;
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;
public:
	inline GraphicsEnvironment(void) {
		winPtr = nullptr;
	}
	inline ~GraphicsEnvironment(void) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	inline GLFWwindow* GetWindow(void) { return winPtr; }

	void Init(unsigned int majorVersion, unsigned int minorVersion);
	bool SetWindow(unsigned int width, unsigned int height, const std::string& title);
	bool InitGlad(void);
	void SetupGraphics(void);
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);

	void CreateRenderer(const std::string& name, std::shared_ptr<Shader> shadPtr, std::shared_ptr<Scene> scenePtr);
	std::shared_ptr<Renderer> GetRenderer(const std::string& name) const;
	void StaticAllocate(void) const;
	void Render(void) const;

	void ProcessInput(void) const;
	static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);

	void Run2D(void) const;
	void Run3D(void) const;
};
