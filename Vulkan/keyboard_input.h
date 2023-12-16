#ifndef KEYBOARD_INPUT
#define KEYBOARD_INPUT
#include <GLFW/glfw3_m.h>
#include <iostream>
class KeyboardInput
{
public:
	void ActivateInput(GLFWwindow* window)
	{
		glfwPollEvents();

		int state_left = glfwGetKey(window, GLFW_KEY_LEFT);
		if (state_left == GLFW_PRESS)
		{
			rotate_y -= 0.05;
		}

		int state_right = glfwGetKey(window, GLFW_KEY_RIGHT);
		if (state_right == GLFW_PRESS)
		{
			rotate_y += 0.05;	
		}
	}
public:
	float rotate_y{};
};
#endif // !KEYBOARD_INPUT

