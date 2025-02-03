#pragma once
#include "Camera.h"
#include "GraphicsStructures.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "Util.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace Cicada {

class GraphicsEnvironment {

private:
	std::shared_ptr<Camera> m_cam;

public:
	inline static GraphicsEnvironment& Instance() {
		static GraphicsEnvironment instance;
		return instance;
	}
	
	inline const std::shared_ptr<Camera>& GetCamera() const { return m_cam; }
	inline void SetCamera(const std::shared_ptr<Camera>& cam) { this->m_cam = cam; }

	void ProcessInput(double elapsedSeconds) const;

	void Run3D(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader);
};

}