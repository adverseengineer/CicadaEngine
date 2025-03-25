#pragma once

#include "GraphicsContext.h"
#include "Log.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace Cicada::UI {

class UISystem {
	
private:
	UISystem() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(GraphicsContext::Instance().GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}
	~UISystem() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

public:
	inline static UISystem& Instance() {
		static UISystem instance;
		return instance;
	}
};

}
