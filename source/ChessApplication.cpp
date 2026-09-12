#include "ChessApplication.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Renderer/Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer/ShaderManager.h"
#include "Renderer/TextureManager.h"

namespace
{
	void APIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void*)
	{
		const std::string_view text( message, static_cast<std::size_t>(length));

		spdlog::debug(
			"OpenGL: type={}, severity={}, message={}",
			type,
			severity,
			text);
	}
}

ChessApplication::ChessApplication(const ApplicationSpecification& spec)
	: Specification(spec)
	, CWindow(nullptr)
	, ScreenWidth(spec.Width)
	, ScreenHeight(spec.Height)
	, FramebufferResized(false)
	, LastMouseX(0.0)
	, LastMouseY(0.0)
{}

ChessApplication::~ChessApplication()
{}

int ChessApplication::Run()
{
	if (!Initialize())
	{
		spdlog::error("ChessApplication initialization failed.");
		return -1;
	}

	MainLoop();

	Uninitialize();

	return 0;
}

bool ChessApplication::Initialize()
{
	InitializeLogger();

	if (!InitializeWindow())
	{
		spdlog::error("Failed to initialize window.");
		return false;
	}
	if (!InitializeOpenGL())
	{
		spdlog::error("Failed to initialize OpenGL.");
		return false;
	}

	InitializeScene();
	InitializeRenderer();

	return true;
}

void ChessApplication::InitializeLogger()
{
	auto logger = spdlog::stdout_color_mt("console");

	logger->set_pattern("%^[%H:%M:%S.%e] [%l]%$ %v");
	logger->set_level(spdlog::level::debug);

	spdlog::set_default_logger(logger);
}

bool ChessApplication::InitializeWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	CWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, Specification.Title.c_str(), NULL, NULL);
	if (CWindow == NULL)
	{
		spdlog::error("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(CWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();
		return false;
	}

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	glfwSetInputMode(CWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(CWindow, this);

	glfwSetFramebufferSizeCallback(CWindow, FramebufferSizeCallback);
	glfwSetCursorPosCallback(CWindow, MouseCallback);
	glfwSetScrollCallback(CWindow, ScrollCallback);
	return true;
}

bool ChessApplication::InitializeOpenGL()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(GLMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
	
	return true;
}

void ChessApplication::InitializeManagers()
{
	TextureManager::Init();
}

void ChessApplication::InitializeScene()
{
	CScene = std::make_unique<Scene>();
	CScene->Initialize();
}

void ChessApplication::InitializeRenderer()
{
	CRenderer = std::make_unique<Renderer>();
	CRenderer->Initialize(CScene.get(), ScreenWidth, ScreenHeight);
}

void ChessApplication::Uninitialize()
{
	UninitializeRenderer();
	UninitializeScene();
	UninitializeOpenGL();
	UninitializeWindow();
	UninitializeLogger();
}

void ChessApplication::UninitializeLogger()
{
	spdlog::shutdown();
}

void ChessApplication::UninitializeWindow()
{
	glfwTerminate();
}

void ChessApplication::UninitializeOpenGL()
{
	
}

void ChessApplication::UninitializeManagers()
{
	TextureManager::ShutDown();
}

void ChessApplication::UninitializeScene()
{}

void ChessApplication::UninitializeRenderer()
{}

void ChessApplication::MainLoop()
{
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	spdlog::info("Starting render loop...");

	// render loop
	while (!glfwWindowShouldClose(CWindow))
	{
		double currFrameTime = glfwGetTime();
		deltaTime = static_cast<float>(currFrameTime - lastFrameTime);
		lastFrameTime = currFrameTime;

		ProcessInput(deltaTime);

		if (FramebufferResized)
		{
			glViewport(0, 0, ScreenWidth, ScreenHeight);
			CRenderer->Resize(ScreenWidth, ScreenHeight);
			FramebufferResized = false;
		}

		CRenderer->Render(CScene.get(), ScreenWidth, ScreenHeight, lastFrameTime, deltaTime);

		glfwSwapBuffers(CWindow);
		glfwPollEvents();
	}
}

void ChessApplication::ProcessInput(float deltaTime)
{
	if (glfwGetKey(CWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(CWindow, true);
	}
	if (glfwGetKey(CWindow, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(CWindow, GLFW_KEY_L) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(CWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		CScene->MainCamera->Move(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(CWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		CScene->MainCamera->Move(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(CWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		CScene->MainCamera->Move(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(CWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		CScene->MainCamera->Move(CameraMovement::RIGHT, deltaTime);
	}
}

ChessApplication* ChessApplication::FromWindow(GLFWwindow* window)
{
	return static_cast<ChessApplication*>(glfwGetWindowUserPointer(window));
}


void ChessApplication::FramebufferSizeCallback(GLFWwindow * window, int width, int height)
{
	ChessApplication* app = FromWindow(window);
	if (!app) return;

	app->ScreenWidth = static_cast<uint32_t>(width);
	app->ScreenHeight = static_cast<uint32_t>(height);
	app->FramebufferResized = true;
}

void ChessApplication::MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	ChessApplication* app = FromWindow(window);
	if (!app) return;

	if (app->FirstMouse)
	{
		app->FirstMouse = false;
		app->LastMouseX = xpos;
		app->LastMouseY = ypos;
	}

	float xoffset = xpos - app->LastMouseX;
	float yoffset = app->LastMouseY - ypos;

	app->LastMouseX = xpos;
	app->LastMouseY = ypos;

	app->CScene->MainCamera->Rotate(xoffset, yoffset);
}

void ChessApplication::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	ChessApplication* app = FromWindow(window);
	if (!app) return;

	app->CScene->MainCamera->Zoom(yoffset);
}
