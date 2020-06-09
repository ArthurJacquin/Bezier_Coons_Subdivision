#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>

#include "Vertex.h"
#include <vector>
#include "Curve.h"
#include "Color.h"
#include "imgui.h"

class Input
{
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 position;

	float horizontalAngle;
	float verticalAngle;

public:
	Input();

	float lastTime;
	Vec3 rotation;

	glm::mat4 getProjectionMatrix() { return ProjectionMatrix; }
	glm::mat4 getViewMatrix() { return ViewMatrix; }

	static void deleteVertex();
	static void deleteCurves();

	static float getDistance(float xa, float ya, Vertex v);
	static void select(float x, float y);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void computeMatricesFromInputs(GLFWwindow* win, GLint winWidth, GLint winHeight);
};