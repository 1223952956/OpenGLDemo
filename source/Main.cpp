#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

#include "Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Camera.h"
#include "Renderer/Model.h"
#include "Renderer/TextureManager.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Renderer/IBLBaker.h"
#include "Renderer/ShaderManager.h"

uint32_t screenWidth = 1920.0f;
uint32_t screenHeight = 1080.0f;
bool framebufferResized = false;
float blend = 0.2f;

float lastMouseX = static_cast<float>(screenWidth) / 2.0f;
float lastMouseY = static_cast<float>(screenHeight) / 2.0f;

Camera MainCamera;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	screenWidth = static_cast<uint32_t>(width);
	screenHeight = static_cast<uint32_t>(height);
	framebufferResized = true;
}

void processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		blend = std::min((blend + 0.02f), 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		blend = std::max((blend - 0.02f), 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		MainCamera.Move(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		MainCamera.Move(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		MainCamera.Move(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		MainCamera.Move(CameraMovement::RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
		firstMouse = false;
		lastMouseX = xpos;
		lastMouseY = ypos;
	}

	float xoffset = xpos - lastMouseX;
	float yoffset = lastMouseY - ypos;

	lastMouseX = xpos;
	lastMouseY = ypos;

	MainCamera.Rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	MainCamera.Zoom(yoffset);
}

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
		<< " type = " << type << ", severity = " << severity
		<< ", message = " << message << std::endl;
}


int main(void)
{
	// Initialize logger
	auto logger = spdlog::stdout_color_mt("console");

	logger->set_pattern("%^[%H:%M:%S.%e] [%l]%$ %v");
	logger->set_level(spdlog::level::debug);

	spdlog::set_default_logger(logger);

	// Initialize OpenGL context and window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		spdlog::error("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	MainCamera.Pos = glm::vec3(0.0f, 0.0f, 3.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(MessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);

	// Initialize
	TextureManager::Init();

	ShaderManager shaderManager;
	shaderManager.Init();

	Scene scene;
	Renderer renderer(shaderManager);

	scene.MainCamera = &MainCamera;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.5f,  3.0f,  2.0f),
		glm::vec3(-3.0f,  1.0f,  2.0f),
		glm::vec3(-2.0f,  2.0f, -3.0f),
		glm::vec3(0.0f,  5.0f, 0.0f)
	};

	// Model Initialize
	auto& ChessBoard = scene.CreatePiece();
	ChessBoard.ModelPtr = std::make_shared<Model>("content/models/ChessBoard.glb");
	auto& BlackQueen = scene.CreatePiece();
	BlackQueen.ModelPtr = std::make_shared<Model>("content/models/BlackQueen.glb");
	auto& WhiteRook = scene.CreatePiece();
	WhiteRook.ModelPtr = std::make_shared<Model>("content/models/WhiteKnight.glb");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
	glm::mat4 model_normal = glm::transpose(glm::inverse(model));
	for (int i = 0; i < scene.Pieces.size(); ++i)
	{
		scene.Pieces[i]->Transform = model;
	}

	// Light Initialize
	scene.DirectionalLights.emplace_back(DirectionalLight(glm::vec3(-2.0f, -2.0f, -1.0f), glm::vec3(1.0f, 0.95f, 0.90f), 2.0f));
	for (int i = 0; i < 4; ++i)
	{
		scene.PointLights.emplace_back(PointLight(pointLightPositions[i], glm::vec3(1.0f, 0.97f, 0.92f), 10.f, 10.0f));
	}
	// scene.SpotLights.emplace_back(SpotLight(MainCamera.Pos, MainCamera.GetFront(), glm::vec3(1.f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), 10.f, 1.f));

	renderer.Init(scene, screenWidth, screenHeight);

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	spdlog::info("Starting render loop...");

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		double currFrameTime = glfwGetTime();
		deltaTime = static_cast<float>(currFrameTime - lastFrameTime);
		lastFrameTime = currFrameTime;

		processInput(window, deltaTime);

		if (framebufferResized)
		{
			glViewport(0, 0, screenWidth, screenHeight);
			renderer.Resize(screenWidth, screenHeight);
			framebufferResized = false;
		}

		renderer.Render(scene, screenWidth, screenHeight, lastFrameTime, deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Release resources
	TextureManager::ShutDown();
	shaderManager.ShutDown();

	glfwTerminate();
	spdlog::shutdown();

	return 0;
}

