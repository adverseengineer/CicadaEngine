#pragma once
#include "Camera.h"
#include "GraphicsStructures.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Util.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

//TODO: move the game loop into main and nuke this stupid class

namespace Cicada {

class GraphicsEnvironment {

private:
	GraphicsEnvironment() = default;
	~GraphicsEnvironment() = default;

public:
	inline static GraphicsEnvironment& Instance() {
		static GraphicsEnvironment instance;
		return instance;
	}
	
	void ProcessInput(double elapsedSeconds) const;

	void Run3D(entt::registry& reg);
};

}