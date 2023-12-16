#ifndef WINDOW_APP_WINDOW
#define WINDOW_APP_WINDOW
#include <GLFW/glfw3_m.h>
class WindowAppWindow
{
public:
	void CreateWindow(int width, int height)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window_handle = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	}
public:
	GLFWwindow* window_handle;
};
#endif // !WINDOW

