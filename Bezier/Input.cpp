#pragma once
#include "Input.h"
#include "Vertex.h"
#include "UI.h"
#include <iostream>
#include <vector>
#include "Color.h"
#include "imgui.h"

# define M_PI           3.14159265358979323846  /* pi */

extern int width, height;
extern std::vector<Vertex> vertices;

void Input::waitForBool()
{

}

void Input::deleteVertex()
{
	vertices.clear();
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void Input::keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}