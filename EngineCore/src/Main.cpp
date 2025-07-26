#define NOMINMAX //this is needed so that rapidjson doesn't shit the bed, idk why
//#define GLFW_INCLUDE_NONE //this is needed so GLFW doesn't try to load the already loaded GLAD

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
#include "ui/UISystem.h"

#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>

using namespace Cicada;
using namespace Cicada::ECS;

static void SetupRegistry(entt::registry& reg) {

	auto diffuseShader = Shader::Create("diffuse", "res/shader/diffuse.vert", "res/shader/diffuse.frag");
	auto toonShader = Shader::Create("toon", "res/shader/toon.vert", "res/shader/toon.frag");
	auto normShader = Shader::Create("norm", "res/shader/norm.vert", "res/shader/norm.frag");

	auto boxMesh = Mesh::Create("boxMesh", "res/geom/cube.obj");
	auto ballMesh = Mesh::Create("ballMesh", "res/geom/uvsphere.obj");
	auto floorMesh = Mesh::Create("floorMesh", "res/geom/plane.obj");

	auto boxTex = Texture2D::Create("boxTex", "res/tex/crate.png");
	auto ballTex = Texture2D::Create("ballTex", "res/tex/not-a-real-texture.png");
	auto floorTex = Texture2D::Create("floorTex", "res/tex/floor.png");

	auto boxMat = Material::Create("boxMat", diffuseShader, boxTex);
	auto ballMat = Material::Create("ballMat", toonShader, boxTex);
	auto floorMat = Material::Create("floorMat", diffuseShader, floorTex);

	auto brentMesh = Mesh::Create("brentMesh", "res/geom/brent.obj");
	auto brentTex = Texture2D::Create("brentTex", "res/tex/br0_tex00.png");
	auto brentMat = Material::Create("brentMat", normShader, nullptr/*brentTex*/);

	auto& names = reg.storage<std::string>();

	auto box = reg.create();
	names.emplace(box, "box");
	reg.emplace<TransformComponent>(box, glm::vec3{ 4.5, 0.5, 4.5 });
	reg.emplace<MeshComponent>(box, boxMesh);
	reg.emplace<MaterialComponent>(box, boxMat);

	auto ball = reg.create();
	names.emplace(ball, "ball");
	reg.emplace<TransformComponent>(ball, glm::vec3{ 0.0, 0.5, 0.0 });
	reg.emplace<MeshComponent>(ball, ballMesh);
	reg.emplace<MaterialComponent>(ball, ballMat);

	auto floor = reg.create();
	names.emplace(floor, "floor");
	reg.emplace<TransformComponent>(floor, glm::vec3{ 0.0, 0.0, 0.0 });
	reg.emplace<MeshComponent>(floor, floorMesh);
	reg.emplace<MaterialComponent>(floor, floorMat);

	auto brent = reg.create();
	names.emplace(brent, "brent");
	reg.emplace<TransformComponent>(brent, glm::vec3{ 0.0, 5.0, 15.0 });
	reg.emplace<MeshComponent>(brent, brentMesh);
	reg.emplace<MaterialComponent>(brent, brentMat);
}


template<typename... Components>
void inspect_entity(entt::registry& registry, entt::entity entity) {
	Log::Info("{}", static_cast<uint32_t>(entity));

	//expand pack to check which known components are attached
	([&] {
		if (registry.any_of<Components>(entity)) {
			const auto& component = registry.get<Components>(entity);
			std::cout << "  Component: " << typeid(Components).name() << '\n';
			Log::Info("{}", typeid(Components).name());

			if constexpr (std::is_same_v<Components, TransformComponent>) {
				Log::Info("Position: ({}, {}, {})", component.localTransform[3][0], component.localTransform[3][1], component.localTransform[3][2]);
			}
			else if constexpr (std::is_same_v<Components, MeshComponent>) {
				Log::Info("Mesh: ()");
			}
			else if constexpr (std::is_same_v<Components, MaterialComponent>) {
				Shader* tempShad = component.ptr->GetShader().get();
				Texture* tempTex = component.ptr->GetTexture().get();
				Log::Info("Material: (Shader: {}, Texture: {})", fmt::ptr(tempShad), fmt::ptr(tempTex));
			}
		}
	}(), ...); //expands over all component types
}

static bool correctGamma = true;
static int targetFps = 60;

static void BuildDebugUI() {

	auto& sm = SceneManager::Instance();
	auto& localLight = sm.GetLight("local");
	auto& globalLight = sm.GetLight("global");
	auto& io = ImGui::GetIO();

	Camera* cam = Camera::GetMainCam();
	auto& camTrans = cam->GetLocalTransform();

	if (!ImGui::Begin("Component Viewer")) { //early out if the frame is collapsed or off-screen
		ImGui::End();
		return;
	}
	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::Text(
		"[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]\n[%.3f %.3f %.3f %.3f]",
		camTrans[0][0], camTrans[1][0], camTrans[2][0], camTrans[3][0],
		camTrans[0][1], camTrans[1][1], camTrans[2][1], camTrans[3][1],
		camTrans[0][2], camTrans[1][2], camTrans[2][2], camTrans[3][2],
		camTrans[0][3], camTrans[1][3], camTrans[2][3], camTrans[3][3]
	);
	ImGui::Checkbox("Correct Gamma", &correctGamma);
	ImGui::SliderInt("Max FPS", &targetFps, 15, 240);
	ImGui::DragFloat3("Local Light Position", glm::value_ptr(localLight.position));
	ImGui::ColorEdit3("Local Light Color", glm::value_ptr(localLight.color));
	ImGui::SliderFloat("Local Intensity", &localLight.intensity, 0, 1);
	ImGui::SliderFloat("Local Attenuation", &localLight.attenuationCoef, 0, 1);
	ImGui::DragFloat3("Global Light Position", glm::value_ptr(globalLight.position));
	ImGui::ColorEdit3("Global Light Color", glm::value_ptr(globalLight.color));
	ImGui::SliderFloat("Global Intensity", &globalLight.intensity, 0, 1);
	ImGui::End();
}


static void ProcessInput(float elapsedSeconds) {

	auto& gc = GraphicsContext::Instance();
	auto handle = gc.GetWindow();
	Camera* cam = Camera::GetMainCam();
	auto& mouse = gc.GetMouse();

	//if the user hits escape, close the window
	if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(handle, true);

	//if the user hits escape, close the window
	if (glfwGetKey(handle, GLFW_KEY_R) == GLFW_PRESS)
		Renderer::ToggleWireframe();

	//DO WASDQE
	glm::mat4 camMat = cam->GetLocalTransform();
	float camSpeed = 10.0f;

	if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
		camMat[3] -= camMat[2] * camSpeed * elapsedSeconds;
	else if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
		camMat[3] += camMat[2] * camSpeed * elapsedSeconds;

	if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
		camMat[3] -= camMat[0] * camSpeed * elapsedSeconds;
	else if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
		camMat[3] += camMat[0] * camSpeed * elapsedSeconds;

	if (glfwGetKey(handle, GLFW_KEY_Q) == GLFW_PRESS)
		camMat[3] -= camMat[1] * camSpeed * elapsedSeconds;
	else if (glfwGetKey(handle, GLFW_KEY_E) == GLFW_PRESS)
		camMat[3] += camMat[1] * camSpeed * elapsedSeconds;

	//really cool zoom effect i found by mistake
	if (glfwGetKey(handle, GLFW_KEY_T) == GLFW_PRESS)
		camMat[3] /= camSpeed * elapsedSeconds + 1.0f;
	else if (glfwGetKey(handle, GLFW_KEY_Y) == GLFW_PRESS)
		camMat[3] *= camSpeed * elapsedSeconds + 1.0f;

	//DO MOUSELOOK:
	//get a theta and phi value from the mouse position
	float theta = glm::radians(90.0f - (180.0f * mouse.x / mouse.windowWidth)); //90 = hard left, -90 = hard right
	float phi = glm::radians(180.0f - (180.0f * mouse.y / mouse.windowHeight)); //180 = straight up, 0 = straight down

	//use the spherical coordinate formula to make a unit vector facing in the corresponding direction
	float sinPhi = sin(phi);
	glm::vec3 zAxis(0.0);
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

	cam->SetLocalTransform(orientation);
	cam->SetPosition(camMat[3]);
}

static void Run3D(entt::registry& reg) {

	glm::vec3 clearColor = { 0.4f, 0.4f, 0.4f };

	auto& context = GraphicsContext::Instance();
	auto handle = context.GetWindow();
	Camera* cam = Camera::GetMainCam();
	auto& sm = SceneManager::Instance();

	auto& globalLight = sm.GetLight("global");
	auto& localLight= sm.GetLight("local");
	
	UniformBufferObject cameraUBO(2 * sizeof(glm::mat4), 2);
	UniformBufferObject lightUBO(96, 3);
	
	Shader::Get("toon")->AttachUBO("CameraData", cameraUBO);
	Shader::Get("norm")->AttachUBO("CameraData", cameraUBO);
	Shader::Get("diffuse")->AttachUBO("CameraData", cameraUBO);
	Shader::Get("diffuse")->AttachUBO("Lights", lightUBO);

	double lastUpdateTime = 0.0;
	double lastFrameTime = 0.0;

	while (!glfwWindowShouldClose(handle)) {

		double now = glfwGetTime();
		double delta = now - lastUpdateTime;
	
		double maxFrameTime = 1.0 / targetFps;
	
		glfwPollEvents();
		ProcessInput(delta);

		//if it's not yet time for the next frame, don't do it
		//everything inside this if is only run once per frame of the target fps
		if(now - lastFrameTime >= maxFrameTime) {

			glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if (correctGamma) glEnable(GL_FRAMEBUFFER_SRGB);
			else glDisable(GL_FRAMEBUFFER_SRGB);

			int width = context.GetWidth();
			int height = context.GetHeight();
			if (width >= height) cam->m_aspectRatio = width / (height * 1.0f);
			else cam->m_aspectRatio = height / (width * 1.0f);

			cam->Update();
			
			auto view = glm::inverse(cam->GetLocalTransform());

			cameraUBO.Write(0, sizeof(glm::mat4), glm::value_ptr(view));
			cameraUBO.Write(64, sizeof(glm::mat4), glm::value_ptr(cam->m_projection));
			
			lightUBO.Write(0, sizeof(glm::vec3), glm::value_ptr(localLight.position));
			lightUBO.Write(16, sizeof(glm::vec3), glm::value_ptr(localLight.color));
			lightUBO.Write(28, sizeof(GLfloat), &localLight.intensity);
			lightUBO.Write(32, sizeof(GLfloat), &localLight.attenuationCoef);
			lightUBO.Write(48, sizeof(glm::vec3), glm::value_ptr(globalLight.position));
			lightUBO.Write(64, sizeof(glm::vec3), glm::value_ptr(globalLight.color));
			lightUBO.Write(76, sizeof(GLfloat), &globalLight.intensity);
			lightUBO.Write(80, sizeof(GLfloat), &globalLight.attenuationCoef);
			
			Renderer::Render(reg);

			//and then render ui separately
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			BuildDebugUI();
			Log::BuildLogWindow();
			
			ImGui::EndFrame(); //not strictly necessary, because render calls this too, but i want to remember what its for
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
			glfwSwapBuffers(handle);
			lastFrameTime = now;
		}
		
		lastUpdateTime = now;
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	Log::Init("log.txt");

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

	auto cam = std::make_shared<Camera>(60.0f, 0.05f, 100.0f, 1200.0f / 800.0f);
	cam->SetMainCam();
	cam->SetPosition({ 0.0f, 5.0f, 15.0f });

	sm.SetLight("local", {
		glm::vec3{ 0, 10.0f, 0 },
		glm::vec3{ 0.7f, 1.0f, 0.8f },
		1.0, 0.0
	});
	sm.SetLight("global", {
		glm::vec3{ 0.0f, 1.0f, 10.0f },
		glm::vec3{ 1.0f, 0.0f, 1.0f },
		1.0, 0.5
	});
	
	entt::registry reg;
	SetupRegistry(reg);

	//rapidjson::Document sceneSchema, sceneJson;
	//JsonUtils::ReadJson("scene_schema.json", sceneSchema);
	//JsonUtils::ReadJson("test_scene.json", sceneJson);

	//bool ye = JsonUtils::ValidateAgainstSchema(sceneSchema, sceneJson);

	//if (ye)
		//Logger::Write(LogEntry::Level::Info, "All clear!");

	//auto& objectsJson = sceneJson["gameObjects"];

	for (auto entity : reg.storage<entt::entity>()) {
		inspect_entity<TransformComponent, MeshComponent, MaterialComponent>(reg, entity);
	}

	Run3D(reg);

	return 0;
}
