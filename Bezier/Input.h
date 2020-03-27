#pragma once
#include <GLFW/glfw3.h>
#pragma once
#include "Vertex.h"
#pragma once
#include <vector>

class Input
{
public:
	static void waitForBool();
	static void deleteVertex();

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};