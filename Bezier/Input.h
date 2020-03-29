#pragma once
#include <GLFW/glfw3.h>

#include "Vertex.h"
#include <vector>
#include "Curve.h"
#include "Color.h"
#include "imgui.h"

class Input
{
public:
	static void waitForBool();
	static void deleteVertex();
	static void deleteCurves();

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};