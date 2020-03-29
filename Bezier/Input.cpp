#pragma once
#include "Input.h"
#include <iostream>
#include <vector>

# define M_PI           3.14159265358979323846  /* pi */

extern int width, height;
extern std::vector<Vertex> vertices;
extern std::vector<Curve> curves;
extern Color choosedColor;
extern float step;
extern bool movingPoint;
extern int selectedPointId;
extern int selectedCurveId;

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

float Input::getDistance(float xa, float ya, Vertex v)
{
	float x = xa - v.x;
	float y = ya - v.y;

	return abs(sqrtf(pow(x, 2) + pow(y, 2)));
}

void Input::selectPoint(float x, float y)
{
	float minDist = 99999.9f;

	for (int i = 0; i < curves.size(); ++i)
	{
		for (int j = 0; j < curves[i].getControlPoints().size(); ++j)
		{
			float d = getDistance(x, y, curves[i].getControlPoints()[j]);
			if (d < minDist)
			{
				minDist = d;
				selectedPointId = j;
				selectedCurveId = i;
			}
		}
	}
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{

		if (xpos > 220)
		{
			Color col(1.0f, 1.0f, 1.0f);
			Vertex newPoint = Vertex(-1.0f + 2 * xpos / width, 1.0f - 2 * ypos / height, col.x, col.y, col.z);
			vertices.push_back(newPoint);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		selectPoint(xpos, ypos);
		movingPoint = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		movingPoint = false;
	}
}

void Input::keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		Curve c(vertices, step, choosedColor);
		curves.push_back(c);
		vertices.clear();
	}
}