#include "EventManager.h"
#include "GraphicsContext.h"
#include "GraphicsEnvironment.h"
#include "Logger.h"
#include "Shader.h"
#include "Timer.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Cicada;

static bool correctGamma = true;

void GraphicsEnvironment::ProcessInput(double elapsedSeconds) const {

	auto handle = GraphicsContext::Instance().GetWindow();

	//if the user hits escape, close the window
	if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(handle, true);

	if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
		m_cam->MoveZ(elapsedSeconds);
	else if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
		m_cam->MoveZ(elapsedSeconds, -1);

	if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
		m_cam->MoveX(elapsedSeconds);
	else if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
		m_cam->MoveX(elapsedSeconds, -1);

	if (glfwGetKey(handle, GLFW_KEY_Q) == GLFW_PRESS)
		m_cam->MoveY(elapsedSeconds);
	else if (glfwGetKey(handle, GLFW_KEY_E) == GLFW_PRESS)
		m_cam->MoveY(elapsedSeconds, -1);
}

void GraphicsEnvironment::Run3D(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Shader>& shader) {

	glm::vec3 clearColor = { 0.1f, 0.1f, 0.1f };

	auto& context = GraphicsContext::Instance();
	auto handle = context.GetWindow();
	auto& io = ImGui::GetIO();
	Timer timer;

	auto crate = ObjectManager::GetObject("crate");

	while (!glfwWindowShouldClose(handle)) {
		double deltaTime = timer.GetElapsedTimeInSeconds();

		EventManager::TriggerEvent("OnUpdate");

		ProcessInput(deltaTime);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		auto spher = context.GetMouse().spherical.ToMat4();
		auto pos = m_cam->GetPosition();
		m_cam->SetLocalTransform(spher);
		m_cam->SetPosition(pos);

		if (correctGamma)
			glEnable(GL_FRAMEBUFFER_SRGB);
		else
			glDisable(GL_FRAMEBUFFER_SRGB);

		int width = context.GetWidth();
		int height = context.GetHeight();
		if (width >= height)
			m_cam->m_aspectRatio = width / (height * 1.0f);
		else
			m_cam->m_aspectRatio = height / (width * 1.0f);

		m_cam->Update();

		auto view = glm::inverse(m_cam->GetLocalTransform());
		Shader::ForEach([&](auto shader) {
			shader->SetMat4("view", view);
			shader->SetMat4("projection", m_cam->m_projection);
		});

		auto& localLight = scene->GetLocalLight();
		auto& globalLight = scene->GetGlobalLight();

		//always make the lightbulb face towards the camera
		auto sprite = ObjectManager::GetObject("lightbulb");
		sprite->RotateToFace(m_cam->GetPosition());
		sprite->SetPosition(scene->GetLocalLight()->position);

		//and finally call render
		Renderer::RenderScene(scene, shader, m_cam);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::StyleColorsClassic();
		ImGui::NewFrame();
		ImGui::Begin("Interactive Graphics");		
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		
		auto& camTrans = m_cam->GetLocalTransform();
		ImGui::Text(
			"[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]",
			camTrans[0][0], camTrans[1][0], camTrans[2][0], camTrans[3][0],
			camTrans[0][1], camTrans[1][1], camTrans[2][1], camTrans[3][1],
			camTrans[0][2], camTrans[1][2], camTrans[2][2], camTrans[3][2],
			camTrans[0][3], camTrans[1][3], camTrans[2][3], camTrans[3][3]
		);

		ImGui::Checkbox("Correct Gamma", &correctGamma);

		ImGui::ColorEdit3("Background Color", glm::value_ptr(clearColor));

		ImGui::ColorEdit3("Local Light Color", glm::value_ptr(localLight->color));
		ImGui::DragFloat3("Local Light Position", glm::value_ptr(localLight->position));
		ImGui::SliderFloat("Local Intensity", &localLight->intensity, 0, 1);
		ImGui::SliderFloat("Local Attenuation", &localLight->attenuationCoef, 0, 1);
		
		ImGui::ColorEdit3("Global Light Color", glm::value_ptr(globalLight->color));
		ImGui::DragFloat3("Global Light Position", glm::value_ptr(globalLight->position));
		ImGui::SliderFloat("Global Intensity", &globalLight->intensity, 0, 1);
		ImGui::SliderFloat("Global Attenuation", &globalLight->attenuationCoef, 0, 1);

		ImGui::End();

		Logger::RenderLog();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(handle);
		glfwPollEvents();
	}
}
