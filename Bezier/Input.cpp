#pragma once
#include "Input.h"
#include "Matrix.h"
#include "Coons.h"
#include "Subdivision.h"
#include <iostream>
#include <vector>

# define M_PI           3.14159265358979323846  /* pi */

extern int width, height;
extern std::vector<Vertex> vertices;
extern std::vector<Curve> curves;
extern std::vector<int> selectedCurves;
extern std::vector<Mesh> meshes;
extern std::vector<Face> faces;

extern Color choosedColor;
extern float u;
extern float v;
extern float iteration;
extern bool movingPoint;
extern int etape;

extern int selectedPointId;
extern int selectedCurveId;
bool selectingCurve;

bool wantToMovePoint;
bool enable3DViewport;
extern bool enableWireframe;
extern bool enableNormal;

void Input::deleteCurves()
{
	curves.clear();
}

Input::Input()
{
	ViewMatrix = glm::mat4();
	ProjectionMatrix = glm::mat4();

	direction = glm::vec3();
	right = glm::vec3();
	up = glm::vec3();
	position = glm::vec3();

	horizontalAngle = 3.14f;
	verticalAngle = 3.14f;
}

void Input::deleteVertex()
{
	vertices.clear();
}

float Input::getDistance(float xa, float ya, Vertex v)
{
	float x = xa - v.x;
	float y = ya - v.y;

	return sqrtf(pow(x, 2) + pow(y, 2));
}

void Input::select(float x, float y)
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
		if (xpos > 310)
		{
			Color col(1.0f, 1.0f, 1.0f);
			Vertex newPoint = Vertex(-1.0f + 2 * xpos / width, 1.0f - 2 * ypos / height, 0.0, col.x, col.y, col.z);
			vertices.push_back(newPoint);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (curves.size() != 0)
		{
			curves[selectedCurveId].setControlPointColor(selectedPointId, Color(1.0f, 1.0f, 1.0f));

			select(-1.0f + 2 * xpos / width, 1.0f - 2 * ypos / height);

			if (selectingCurve)
			{
				if (std::find(selectedCurves.begin(), selectedCurves.end(), selectedCurveId) == selectedCurves.end())
				{
					selectedCurves.push_back(selectedCurveId);
					curves[selectedCurveId].setCurveColor(Color(0.0f, 1.0f, 0.0f));
				}
			}
			else if(wantToMovePoint)
			{
				curves[selectedCurveId].setControlPointColor(selectedPointId, Color(0.0f, 1.0f, 0.0f));
				movingPoint = true;
			}
			else 
			{
				curves[selectedCurveId].setControlPointColor(selectedPointId, Color(0.0f, 1.0f, 0.0f));
			}
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		if (movingPoint)
		{
			movingPoint = false;
			curves[selectedCurveId].setControlPointColor(selectedPointId, Color(1.0f, 1.0f, 1.0f));
		}
	}
}

void Input::keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS || key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS)
	{
		if (vertices.size() > 2)
		{
			Curve c(vertices, u, v, iteration, choosedColor);
			curves.push_back(c);
			vertices.clear();
		}
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		selectingCurve = true;
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
	{
		selectingCurve = false;
	}

	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
	{
		wantToMovePoint = true;
	}

	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE)
	{
		wantToMovePoint = false;
	}

	if (key == GLFW_KEY_KP_ADD && action == GLFW_RELEASE)
	{
		curves[selectedCurveId].addControlPointAtIndex(selectedPointId + 1, curves[selectedCurveId].getControlPoints()[selectedPointId]);
	}

	//Clear
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		if(selectedCurveId != NULL)
			curves[selectedCurveId].setControlPointColor(selectedPointId, Color(1.0f, 1.0f, 1.0f));
		selectedCurveId = NULL;
		selectedPointId = NULL;
		
		for(int i = 0; i < selectedCurves.size(); i++)
			curves[selectedCurves[i]].setCurveColor(choosedColor);
		
		selectedCurves.clear();

		meshes.clear();
		faces.clear();
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		Matrix t = Matrix::Translate(0.1f, 0.1f);
		for(int i = 0; i < selectedCurves.size(); i++)
			curves[selectedCurves[i]].Transform(t);
	}

	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		enable3DViewport = !enable3DViewport;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		enableWireframe = !enableWireframe;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		enableNormal = !enableNormal;
	}

	//Subdivision
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		faces = DooSabin(faces);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		faces = CatmullClark(faces);
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		faces = LoopAlgo(faces);
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		faces = Kobelt(faces);
	}

	//Etape
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		etape = 0;
		meshes.clear();
		meshes.push_back(generateCoon(curves, u, v));
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		etape = 1;
		meshes.clear();
		meshes.push_back(generateCoon(curves, u, v));
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		etape = 2;
		meshes.clear();
		meshes.push_back(generateCoon(curves, u, v));
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		etape = 3;
		meshes.clear();
		meshes.push_back(generateCoon(curves, u, v));
	}
}

void Input::computeMatricesFromInputs(GLFWwindow* win, GLint winWidth, GLint winHeight)
{
	float speed = 5.0f;
	float mouseSpeed = 0.5f;

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	if(enable3DViewport)
		glfwSetCursorPos(win, winWidth / 2, winHeight / 2);

	horizontalAngle -= mouseSpeed * deltaTime * float(winWidth / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(winHeight / 2 - ypos);

	//Position
	//Forward
	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Backward
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Right
	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Left
	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwDestroyWindow(win);
		exit(0);
	}

	if (enable3DViewport)
	{
		direction = glm::vec3( 
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
	}
	else
	{
		position = glm::vec3();
		direction = glm::vec3(0, 0, -1);
		right = glm::vec3(1, 0, 0);
	}

	up = glm::cross(right, direction);

	//Matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.f), float(winWidth) / float(winHeight), 0.01f, 100.0f);

	ViewMatrix = glm::lookAt(
		position,
		position + direction,
		up
	);
}