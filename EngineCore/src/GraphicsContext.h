#pragma once
#include "GraphicsStructures.h"
#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>
#include <string>

namespace Cicada {

class GraphicsContext {
private:
	GLFWwindow* m_windowHandle = nullptr;
	int m_windowWidth = 0, m_windowHeight = 0;
	MouseParams m_mouse;

	static bool gladInitialized;

	GraphicsContext();
	~GraphicsContext();
	
	static void OnWindowResize(GLFWwindow* window, int width, int height);
	static void OnMouseMove(GLFWwindow* window, double mouseX, double mouseY);
	static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);

public:
	inline static GraphicsContext& Instance() {
		static GraphicsContext instance;
		return instance;
	}

	inline GLFWwindow* GetWindow() const { return m_windowHandle; }
	inline const MouseParams& GetMouse() const { return m_mouse; }
	inline unsigned int GetWidth() const { return m_windowWidth; }
	inline unsigned int GetHeight() const { return m_windowHeight; }
	inline void SetWidth(unsigned int width) { m_windowWidth = width; }
	inline void SetHeight(unsigned int height) { m_windowHeight = height; }

	void CreateWindow(unsigned int width, unsigned int height, const std::string& title);
	void InitGlad();

	void Configure();
	void RegisterCallbacks();
};

}
