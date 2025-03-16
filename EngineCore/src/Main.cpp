#define NOMINMAX //this is needed so that rapidjson doesn't shit the bed, idk why

#include "Camera.h"
#include "ecs/components/MaterialComponent.h"
#include "ecs/components/MeshComponent.h"
#include "ecs/components/TransformComponent.h"
#include "EventManager.h"
#include "FileSystem.h"
#include "GraphicsContext.h"
#include "JsonUtils.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "ScriptManager.h"
#include "Timer.h"
#include "ui/UISystem.h"
#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>

#ifdef CreateWindow //fuck you microsoft, i want this function name
#undef CreateWindow
#endif

using namespace Cicada;
using namespace Cicada::ECS;

void DoUBOShit() {
	
}

glm::vec3 Hsv2Rgb(glm::vec3 hsv) {
	
	glm::vec3 rgb { 0.0 };
	
	int i = hsv.x * 6;
	int f = hsv.x * 6 - i;
	float p = hsv.z * (1 - hsv.y);
	float q = hsv.z * (1 - f * hsv.y);
	float t = hsv.z * (1 - (1 - f) * hsv.y);

	switch (i % 6) {
	case 0: rgb = { hsv.z, t, p }; break;
	case 1: rgb = { q, hsv.z, p }; break;
	case 2: rgb = { p, hsv.z, t }; break;
	case 3: rgb = { p, q, hsv.z }; break;
	case 4: rgb = { t, p, hsv.z }; break;
	case 5: rgb = { hsv.z, p, q }; break;
	}

	return rgb;
}

void SetupRegistry(entt::registry& reg) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");

	auto crateMesh = Mesh::Create("crateMesh", "temp/cube.obj");
	auto ballMesh = Mesh::Create("ballMesh", "temp/uvsphere.obj");
	auto floorMesh = Mesh::Create("floorMesh", "temp/plane.obj");

	auto crateTex = Texture2D::Create("crateTex", "crate.png");
	auto ballTex = Texture2D::Create("ballTex", "not-a-real-texture.png");
	auto floorTex = Texture2D::Create("floorTex", "floor.png");

	auto crateMat = Material::Create("crateMat", diffuseShader, crateTex);
	auto ballMat = Material::Create("ballMat", diffuseShader, ballTex);
	auto floorMat = Material::Create("floorMat", diffuseShader, floorTex);

	auto& names = reg.storage<std::string>();

	auto crate = reg.create();
	names.emplace(crate, "crate");
	reg.emplace<TransformComponent>(crate, glm::vec3{ 10.0, 0.0, 10.0 });
	reg.emplace<MeshComponent>(crate, crateMesh);
	reg.emplace<MaterialComponent>(crate, crateMat);

	auto ball = reg.create();
	names.emplace(ball, "ball");
	reg.emplace<TransformComponent>(ball, glm::vec3{ -10.0, 0.0, 10.0 });
	reg.emplace<MeshComponent>(ball, ballMesh);
	reg.emplace<MaterialComponent>(ball, ballMat);

	auto floor = reg.create();
	names.emplace(floor, "floor");
	reg.emplace<TransformComponent>(floor, glm::vec3{ 0.0, 0.0, 0.0 });
	reg.emplace<MeshComponent>(floor, floorMesh);
	reg.emplace<MaterialComponent>(floor, floorMat);

	auto sunMesh = Mesh::Create("sunMesh", "temp/plane.obj");
	auto sunTex = Texture2D::Create("sunTex", "gw.png");
	auto sunMat = Material::Create("sunMat", diffuseShader, sunTex);

	auto sun = reg.create();
	names.emplace(sun, "sun");
	reg.emplace<TransformComponent>(sun, glm::vec3{ 0.0, 20.0, 0.0 });
	reg.emplace<MeshComponent>(sun, sunMesh);
	reg.emplace<MaterialComponent>(sun, sunMat);
}

template<typename... Components>
void inspect_entity(entt::registry& registry, entt::entity entity) {
	Logger::Writef(LogEntry::Level::Info, "{}", static_cast<uint32_t>(entity));

	//expand pack to check which known components are attached
	([&] {
		if (registry.any_of<Components>(entity)) {
			const auto& component = registry.get<Components>(entity);
			std::cout << "  Component: " << typeid(Components).name() << '\n';
			Logger::Writef(LogEntry::Level::Info, "{}", typeid(Components).name());

			if constexpr (std::is_same_v<Components, TransformComponent>) {
				Logger::Writef(LogEntry::Level::Info, "Position: ({}, {}, {})", component.localTransform[3][0], component.localTransform[3][1], component.localTransform[3][2]);
			}
			else if constexpr (std::is_same_v<Components, MeshComponent>) {
				Logger::Writef(LogEntry::Level::Info, "Mesh: ()");
			}
			else if constexpr (std::is_same_v<Components, MaterialComponent>) {
				Shader* tempShad = component.ptr->GetShader().get();
				Texture* tempTex = component.ptr->GetTexture().get();
				Logger::Writef(LogEntry::Level::Info, "Material: (Shader: {}, Texture: {})", fmt::ptr(tempShad), fmt::ptr(tempTex));
			}
		}
	}(), ...); //expands over all component types
}

static bool correctGamma = true;

void ProcessInput(double elapsedSeconds) {

	auto handle = GraphicsContext::Instance().GetWindow();
	Camera* cam = Camera::GetMainCam();

	//if the user hits escape, close the window
	if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(handle, true);

	if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
		cam->MoveZ_OLD(elapsedSeconds);
	else if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
		cam->MoveZ_OLD(elapsedSeconds, -1);

	if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
		cam->MoveX_OLD(elapsedSeconds);
	else if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
		cam->MoveX_OLD(elapsedSeconds, -1);

	if (glfwGetKey(handle, GLFW_KEY_Q) == GLFW_PRESS)
		cam->MoveY_OLD(elapsedSeconds);
	else if (glfwGetKey(handle, GLFW_KEY_E) == GLFW_PRESS)
		cam->MoveY_OLD(elapsedSeconds, -1);
}

static glm::mat4 GetLookFromMouse(const MouseParams& mouse) {
	
	float theta = glm::radians(90.0f - (180 * mouse.x / mouse.windowWidth)); //left/right
	float phi = glm::radians(180.0f - (180 * mouse.y / mouse.windowHeight)); //up/down

	float sinPhi = sin(phi);
	glm::vec3 zAxis{};
	zAxis.x = sin(theta) * sinPhi;
	zAxis.y = cos(phi);
	zAxis.z = cos(theta) * sinPhi;

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);
	glm::mat4 orientation(1.0f);
	orientation[0] = glm::vec4(xAxis, 0.0f);
	orientation[1] = glm::vec4(yAxis, 0.0f);
	orientation[2] = glm::vec4(zAxis, 0.0f);
	return orientation;
}

void Run3D(entt::registry& reg) {

	glm::vec3 clearColor = { 0.1f, 0.1f, 0.1f };

	auto& context = GraphicsContext::Instance();
	auto handle = context.GetWindow();
	auto& io = ImGui::GetIO();
	Timer timer;

	auto& sm = SceneManager::Instance();

	auto shader = Shader::Get("diffuse");

	while (!glfwWindowShouldClose(handle)) {
		double deltaTime = timer.GetElapsedTimeInSeconds();

		//EventManager::TriggerEvent("OnUpdate");

		ProcessInput(deltaTime);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		Camera* cam = Camera::GetMainCam();

		auto camRot = GetLookFromMouse(context.GetMouse());
		auto camPos = cam->GetPosition();
		cam->SetLocalTransform(camRot);
		cam->SetPosition(camPos);

		if (correctGamma) glEnable(GL_FRAMEBUFFER_SRGB);
		else glDisable(GL_FRAMEBUFFER_SRGB);

		int width = context.GetWidth();
		int height = context.GetHeight();
		if (width >= height) cam->m_aspectRatio = width / (height * 1.0f);
		else cam->m_aspectRatio = height / (width * 1.0f);

		cam->Update();

		auto view = glm::inverse(cam->GetLocalTransform());
		Shader::ForEach([&](auto shader) {
			shader->SetMat4("view", view);
			shader->SetMat4("projection", cam->m_projection);
			});

		auto& localLight = sm.GetLight("local");
		auto& globalLight = sm.GetLight("global");

		//always make the lightbulb face towards the camera
		//auto sprite = ObjectManager::GetObject("lightbulb");
		//sprite->RotateToFace(m_cam->GetPosition());
		//sprite->SetPosition(scene->GetLocalLight()->position);

		//and finally call render
		//Renderer::RenderScene(scene, shader, cam);
		Renderer::Render(reg);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::StyleColorsClassic();
		ImGui::NewFrame();
		ImGui::Begin("Interactive Graphics");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		auto& camTrans = cam->GetLocalTransform();
		ImGui::Text(
			"[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]",
			camTrans[0][0], camTrans[1][0], camTrans[2][0], camTrans[3][0],
			camTrans[0][1], camTrans[1][1], camTrans[2][1], camTrans[3][1],
			camTrans[0][2], camTrans[1][2], camTrans[2][2], camTrans[3][2],
			camTrans[0][3], camTrans[1][3], camTrans[2][3], camTrans[3][3]
		);

		ImGui::Checkbox("Correct Gamma", &correctGamma);

		ImGui::ColorEdit3("Background Color", glm::value_ptr(clearColor));

		ImGui::ColorEdit3("Local Light Color", glm::value_ptr(localLight.color));
		ImGui::DragFloat3("Local Light Position", glm::value_ptr(localLight.position));
		ImGui::SliderFloat("Local Intensity", &localLight.intensity, 0, 1);
		ImGui::SliderFloat("Local Attenuation", &localLight.attenuationCoef, 0, 1);

		ImGui::ColorEdit3("Global Light Color", glm::value_ptr(globalLight.color));
		ImGui::DragFloat3("Global Light Position", glm::value_ptr(globalLight.position));
		ImGui::SliderFloat("Global Intensity", &globalLight.intensity, 0, 1);
		ImGui::SliderFloat("Global Attenuation", &globalLight.attenuationCoef, 0, 1);

		ImGui::End();

		Logger::RenderLog();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(handle);
		glfwPollEvents();
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	//ScriptManager::Init();
	//ScriptManager::LoadScript("test.lua");

	//EventManager::TriggerEvent("OnStart");

	auto& gc = GraphicsContext::Instance();
	gc.CreateWindow(1200, 800, "Cicada Engine");
	gc.InitGlad();
	gc.Configure();

	//need to initialize UI after window is made
	auto& ui = UI::UISystem::Instance();

	auto& sm = SceneManager::Instance();

	auto cam = std::make_shared<Camera>(60.0f, 0.01f, 500.0f, 1200.0f / 800.0f);
	cam->SetMainCam();
	cam->SetPosition({ 0.0f, 15.0f, 30.0f });

	Light local = {
		glm::vec3{ 0, 10.0f, 0 },
		glm::vec3{ 0.3f, 0.9f, 0.4f },
		1.0, 0.0
	};
	sm.SetLight("local", local);

	Light global = {
		glm::vec3{ 40.0f, 40.0f, 40.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		1.0, 0.5
	};
	sm.SetLight("global", global);
	
	entt::registry reg;
	SetupRegistry(reg);

	//rapidjson::Document sceneSchema, sceneJson;
	//JsonUtils::ReadJson("scene_schema.json", sceneSchema);
	//JsonUtils::ReadJson("test_scene.json", sceneJson);

	//bool ye = JsonUtils::ValidateAgainstSchema(sceneSchema, sceneJson);

	//if (ye)
		//Logger::Write(LogEntry::Level::Info, "All clear!");

	//auto& objectsJson = sceneJson["gameObjects"];

	auto diffuseShader = Shader::Create("diffuse", "diffuse.vert.glsl", "diffuse.frag.glsl");
	
	for (auto entity : reg.storage<entt::entity>()) {
		inspect_entity<TransformComponent, MeshComponent, MaterialComponent>(reg, entity);
	}

	Run3D(reg);

	return 0;
}
