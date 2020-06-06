#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLShader.h"

#if defined(_WIN32) && defined(_MSC_VER)
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")	
#pragma comment(lib, "opengl32.lib")
#elif defined(__APPLE__)
#elif defined(__linux__)
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include "Vertex.h"
#include "Input.h"
#include "OpenGLcore.h"

#include "imgui.h"
#include "examples\\imgui_impl_opengl3.h"
#include "examples\\imgui_impl_glfw.h"

#include "imgui.cpp"
#include "examples\\imgui_impl_glfw.cpp"
#include "examples\\imgui_impl_opengl3.cpp"
#include "imgui_draw.cpp"
#include "imgui_widgets.cpp"
#include "imgui_demo.cpp"

#include "Color.h"

const char* glsl_version = "#version 150";

//Variables globales
GLShader BasicShader;
GLuint VAO;
uint32_t VBOCurrent;
Input input;

//tableau de positions du tableau en cours
std::vector<Vertex> vertices;
std::vector<Curve> curves;
std::vector<Mesh> meshes;
int totalSize = 0;

bool movingPoint;
int selectedPointId;
int selectedCurveId;
std::vector<int> selectedCurves;

Color choosedColor(1.f, 0.f, 0.f);
int width = 1200;
int height = 800;
float step = 0.05f;

bool enableWireframe;

bool Initialise() {

	//Init GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "erreur d'initialisation de GLEW!"
			<< std::endl;
	}

	std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;

	//Init shaders
	BasicShader.LoadVertexShader("Basic.vs");
	BasicShader.LoadFragmentShader("Basic.fs");
	BasicShader.Create();

	//Init Program
	GLuint64 BasicProgram = BasicShader.GetProgram();
	glUseProgram(BasicProgram);
	/*
#ifdef WIN32 
	wglSwapIntervalEXT(1);*/
//#endif 

	return true;
}

void updateVBO()
{
	//Création VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOCurrent);
	
	//Position
	int loc_position = glGetAttribLocation(BasicShader.GetProgram(), "a_position");
	glVertexAttribPointer(loc_position, 2, GL_DOUBLE, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(loc_position);
	
	//Color
	int loc_color = glGetAttribLocation(BasicShader.GetProgram(), "a_color");
	glVertexAttribPointer(loc_color, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, r));
	glEnableVertexAttribArray(loc_color);

	//Normals
	int loc_normal = glGetAttribLocation(BasicShader.GetProgram(), "a_normal");
	glVertexAttribPointer(loc_normal, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(loc_normal);
}

void Terminate() 
{
	BasicShader.Destroy();
	DeleteBufferObject(VBOCurrent);
	glDeleteVertexArrays(1, &VAO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Display(GLFWwindow* window)
{
	glfwGetWindowSize(window, &width, &height);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	// Desactive le "scissoring"
	glDisable(GL_SCISSOR_TEST);
	// Defini le viewport en pleine fenetre
	glViewport(0, 0, width, height);

	glLineWidth(1.f);
	glEnable(GL_PROGRAM_POINT_SIZE);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (movingPoint)
	{
		vector<Vertex> v = curves[selectedCurveId].getControlPoints();
		v[selectedPointId].setPositionUsingMouse(xpos, ypos);
		curves[selectedCurveId].setControlPoints(v);
		curves[selectedCurveId].updateCurve();
	}


	//Création VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Draw vertices
	VBOCurrent = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * vertices.size(), vertices.data());
	updateVBO();
	
	if(vertices.size() < 2)
		glDrawArrays(GL_POINTS, 0, vertices.size());
	else
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	//Draw curves
	for (int i = 0; i < curves.size(); ++i)
	{
		VBOCurrent = curves[i].getVBOControl();
		updateVBO();

		/* Render here */
		glDrawArrays(GL_LINE_STRIP, 0, curves[i].getControlPoints().size());

		VBOCurrent = curves[i].getVBOCurve();
		updateVBO();

		/* Render here */
		glDrawArrays(GL_LINE_STRIP, 0, curves[i].getCurvePoints().size());
	}

	//Draw Meshes
	for (int i = 0; i < meshes.size(); ++i)
	{
		VBOCurrent = meshes[i].getVBO();
		updateVBO();

		glCullFace(GL_FRONT_AND_BACK);

		if(enableWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, meshes[i].getIndices().size(), GL_UNSIGNED_INT, meshes[i].getIndices().data());
	}

	//Désactivation des buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InitialiseGUI(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsClassic();
}

void displayGUI()
{
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(230, 720));
	// render your GUI
	ImGui::Begin("Bezier", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::TextColored(ImVec4(0.9, 0.1, 0.1, 1.0), "  Bienvenue dans Bezier ");
	ImGui::Separator();

	static float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	ImGui::Text("");
	ImGui::Text("   Choississez la couleur ");
	ImGui::Text("       de la courbe: ");
	if (ImGui::ColorEdit3("Color", color))
	{
		choosedColor = color;
	}

	ImGui::Text(" ");
	ImGui::Text("   Choississez le pas ");
	ImGui::Columns(1);
	ImGui::Text("Pas :");
	ImGui::SameLine();
	ImGui::Text("%.2f", step);
	ImGui::SameLine();
	if (ImGui::Button("-"))
	{
		step = step - 0.01f;
		if (step < 0.01)
			step = 0.01;
	}
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		step = step + 0.01f;
		if (step > 1)
			step = 1;
	}
	ImGui::Separator();
	ImGui::Text("   Liste des courbes :");
	for (int i = 0; i < curves.size(); i++)
	{
		ImGui::Text("Courbe %d", i);
	}

	if (curves.size() >= 1)
	{
		static int i = 0;
		ImGui::InputInt("", &i, 0, 50);
		ImGui::SameLine();
		if (ImGui::Button("suppr"))
		{
			curves.erase(curves.begin() + i);
			selectedCurves.clear();
		}
		if (ImGui::Button("supprimer toutes les courbes"))
		{
			curves.clear();
			selectedCurves.clear();
		}
	}

	ImGui::Separator();
	ImGui::Text("");
	ImGui::Text("  Pour selectionner un point :");
	ImGui::Text("  clic droit");
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");
	ImGui::Text("  Pour deplacer un point :");
	ImGui::Text("  Alt + clic droit");
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");
	ImGui::Text("  Pour selectionner une courbe :");
	ImGui::Text("  ctlr + clic droit");
	ImGui::Text("");
	ImGui::Separator();

	if (ImGui::Button("raccordement"))
	{
		if (selectedCurves.size() > 1)
		{
			Curve* firstCurve = &curves[selectedCurves[0]];

			for (int i = 1; i < selectedCurves.size(); ++i)
			{
				firstCurve->link(curves[selectedCurves[i]]);
			}

			for (int i = 1; i < selectedCurves.size(); ++i)
			{
				curves.erase(curves.begin() + selectedCurves[i]);
			}

			firstCurve->setCurveColor(choosedColor);
		}

		selectedCurveId = NULL;
		selectedPointId = NULL;
		selectedCurves.clear();
	}

	ImGui::Separator();
	ImGui::Text("");
	static float translateUI[2] = { 0.0f, 0.0f };
	ImGui::InputFloat2("Translate", translateUI);
	if (ImGui::Button("Translate curve now !"))
	{
		Matrix t = Matrix::Translate(translateUI[0], translateUI[1]);
		for (int i = 0; i < selectedCurves.size(); i++)
			curves[selectedCurves[i]].Transform(t);
	}

	ImGui::Text("");
	static float rotateUI = 0.0f;
	ImGui::InputFloat("Rotate", &rotateUI);
	if (ImGui::Button("Rotate curve now !"))
	{
		Matrix t = Matrix::Rotate(rotateUI);
		
		for (int i = 0; i < selectedCurves.size(); i++)
		{
			float factor[2] = { 0.0f, 0.0f };
			for (int j = 0; j < curves[selectedCurves[i]].getControlPoints().size(); j++)
			{
				factor[0] = factor[0] + curves[selectedCurves[i]].getControlPoints()[j].x;
				factor[1] = factor[1] + curves[selectedCurves[i]].getControlPoints()[j].y;
			}

			factor[0] = factor[0] / curves[selectedCurves[i]].getControlPoints().size();
			factor[1] = factor[1] / curves[selectedCurves[i]].getControlPoints().size();

			Matrix z = Matrix::Translate(-factor[0], -factor[1]);
			Matrix zBis = Matrix::Translate(factor[0], factor[1]);
			curves[selectedCurves[i]].Transform(z);
			curves[selectedCurves[i]].Transform(t);
			curves[selectedCurves[i]].Transform(zBis);
		}
	}

	ImGui::Text("");

	//Scale
	static float scaleUI = 0.0f;
	ImGui::InputFloat("Scale", &scaleUI);
	if (ImGui::Button("Scale curve now !"))
	{
		Matrix t = Matrix::Scale(scaleUI);
		for (int i = 0; i < selectedCurves.size(); i++)
			curves[selectedCurves[i]].Transform(t);
	}

	ImGui::Text("");

	//Extrusion
	if (ImGui::Button("Extrude !"))
	{
		if (selectedCurves.size() != 0)
		{
			for (int i = 0; i < selectedCurves.size(); i++)
			{
				meshes.push_back(curves[selectedCurves[i]].SimpleExtrude(2, 0.5f, 0.2));
				curves.erase(curves.begin() + selectedCurves[i]);
			}
		}
	}

	//Extrusion
	if (ImGui::Button("Revolution !"))
	{
		if (selectedCurves.size() != 0)
		{
			for (int i = 0; i < selectedCurves.size(); i++)
			{
				meshes.push_back(curves[selectedCurves[i]].Revolution(0.01f));
				curves.erase(curves.begin() + selectedCurves[i]);
			}
		}
	}

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");

	if (ImGui::Button("Wireframe"))
	{
		enableWireframe = !enableWireframe;
	}

	ImGui::Text("");

	//deselectionner
	if (ImGui::Button("deselectionner"))
	{
		if (curves.size() > 0)
		{
			curves[selectedCurveId].setControlPointColor(selectedPointId, Color(1.0f, 1.0f, 1.0f));
			selectedCurveId = NULL;
			selectedPointId = NULL;

			for (int i = 0; i < selectedCurves.size(); i++)
				curves[selectedCurves[i]].setCurveColor(choosedColor);

			selectedCurves.clear();
		}
	}

	ImGui::End();

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Beziers", NULL, NULL);
	InitialiseGUI(window);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// toutes nos initialisations vont ici
	Initialise();

	//Input
	glfwSetMouseButtonCallback(window, &input.mouse_button_callback);
	glfwSetKeyCallback(window, &input.keyboard_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwGetFramebufferSize(window, &width, &height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		Display(window);

		displayGUI();

		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();

	}

	// ne pas oublier de liberer la memoire etc...
	Terminate();

	glfwTerminate();
	return 0;
}

