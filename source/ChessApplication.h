#pragma once
#include <cstdint>
#include <string>
#include <memory>


struct GLFWwindow;
class Renderer;
class Scene;


struct ApplicationSpecification
{
	std::string Title = "OpenGL Demo";
	uint32_t Width = 1920;
	uint32_t Height = 1080;

};


class ChessApplication
{
public:
	ChessApplication(const ApplicationSpecification& spec);
	~ChessApplication();

	int Run();

private:
	bool Initialize();
	void InitializeLogger();
	bool InitializeWindow();
	bool InitializeOpenGL();
	void InitializeManagers();
	void InitializeScene();
	void InitializeRenderer();

	void Uninitialize();
	void UninitializeLogger();
	void UninitializeWindow();
	void UninitializeOpenGL();
	void UninitializeManagers();
	void UninitializeScene();
	void UninitializeRenderer();

	void MainLoop();
	void ProcessInput(float deltaTime);

	static ChessApplication* FromWindow(GLFWwindow* window);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	ApplicationSpecification Specification;
	GLFWwindow* CWindow = nullptr;

	uint32_t ScreenWidth;
	uint32_t ScreenHeight;
	bool FramebufferResized;

	bool FirstMouse = true;
	double LastMouseX;
	double LastMouseY;

	std::unique_ptr<Renderer> CRenderer;
	std::unique_ptr<Scene> CScene;
};

