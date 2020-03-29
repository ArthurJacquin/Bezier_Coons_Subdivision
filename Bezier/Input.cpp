#pragma once
#include "Input.h"
#include "UI.h"
#include <iostream>
#include <vector>

# define M_PI           3.14159265358979323846  /* pi */

extern int width, height;
extern std::vector<Vertex> vertices;
extern std::vector<Curve> curves;

void Input::waitForBool()
{

}

void Input::deleteCurves()
{
	curves.clear();
}

void Input::deleteVertex()
{
	vertices.clear();
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (xpos > 220)
		{
			Color col(1.0f, 1.0f, 1.0f);
			Vertex newPoint = Vertex(-1.0f + 2 * xpos / width, 1.0f - 2 * ypos / height, col.x, col.y, col.z);
			vertices.push_back(newPoint);
		}
	}
}

void Input::keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		Curve c(vertices, 0.005f);
		curves.push_back(c);

		vertices.clear();
	}
}