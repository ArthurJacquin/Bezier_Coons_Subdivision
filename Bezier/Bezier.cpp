#pragma region INCLUDES

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
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
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
#include "Coons.h"
#include "MeshGeneration.h"
#include "Subdivision.h"

const char* glsl_version = "#version 150";

#pragma endregion

#pragma region GLOBAL_VARIABLES

//Variables globales
GLShader BasicShader;
GLuint VAO;
uint32_t VBOCurrent;
Input input;

//tableau de positions du tableau en cours
std::vector<Vertex> vertices;
std::vector<Curve> curves;
std::vector<Mesh> meshes;
std::vector<Face> faces;

int totalSize = 0;

bool movingPoint;
int selectedPointId;
int selectedCurveId;
std::vector<int> selectedCurves;

Color choosedColor(1.f, 0.f, 0.f);
int width = 1600;
int height = 800;
float u = 1.0f / 3.0f;
float v = 1.0f / 4.0f;
float iteration = 1.0;
float step = 1.0;

float extrudeHeight = 2;
float extrudeScale = 0.5f;
float extrudeStep = 0.2f;
float revolutionStep = 0.02f;

bool enableWireframe;
extern bool enable3DViewport;
bool enableNormal;

int enableNormalLocation;
int enable3DViewportLocation;
int modelMatrixLocation;
int viewMatrixLocation;
int projectionMatrixLocation;
int cameraPos_location;

extern int etape;

#pragma endregion

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
	BasicShader.LoadVertexShader("Basic.vs.glsl");
	BasicShader.LoadFragmentShader("Basic.fs.glsl");
	BasicShader.Create();

	//Init Program
	GLuint64 BasicProgram = BasicShader.GetProgram();
	glUseProgram(BasicProgram);

	modelMatrixLocation = glGetUniformLocation(BasicProgram, "u_modelMatrix");
	viewMatrixLocation = glGetUniformLocation(BasicProgram, "u_viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(BasicProgram, "u_projectionMatrix");
	
	enable3DViewportLocation = glGetUniformLocation(BasicProgram, "u_enable3DViewport");
	enableNormalLocation = glGetUniformLocation(BasicProgram, "u_enabledNormal");
	cameraPos_location = glGetUniformLocation(BasicProgram, "u_camPos");

	return true;
}

void updateVBO()
{
	//Création VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOCurrent);
	
	//Position
	int loc_position = glGetAttribLocation(BasicShader.GetProgram(), "a_position");
	glVertexAttribPointer(loc_position, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), 0);
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
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Desactive le "scissoring"
	glDisable(GL_SCISSOR_TEST);
	// Defini le viewport en pleine fenetre
	glViewport(0, 0, width, height);

	glLineWidth(2.f);
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

	input.computeMatricesFromInputs(window, width, height);

	glm::mat4 view = input.getViewMatrix();
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = input.getProjectionMatrix();
	glm::mat4 ortho = glm::ortho(-1, 1, -1, 1, -1, 1);

	if(enable3DViewport)
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projection[0][0]);
	else
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &ortho[0][0]);

	//Shading
	glUniform1f(enableNormalLocation, enableNormal);
	glUniform1f(enable3DViewportLocation, enable3DViewport);
	glUniform3fv(cameraPos_location, 1, &input.getPosition()[0]);

	VBOCurrent = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * vertices.size(), vertices.data());
	updateVBO();
	
	glLineWidth(1.f);
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
		glLineWidth(3.f);
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

	//Draw faces
	for (int i = 0; i < faces.size(); ++i)
	{
		VBOCurrent = faces[i].getVBO();
		updateVBO();

		glCullFace(GL_FRONT_AND_BACK);

		if (enableWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, faces[i].getIndices().size(), GL_UNSIGNED_INT, faces[i].getIndices().data());
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
	ImGui::SetNextWindowSize(ImVec2(300, 600));
	// render your GUI
	ImGui::Begin("Curves", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::TextColored(ImVec4(0.9, 0.1, 0.1, 1.0), "  Welcome in math world ! ");
	ImGui::Separator();

	//Parameters
	static float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	ImGui::Text("");
	ImGui::Text("   Choose color ");
	if (ImGui::ColorEdit3("Color", color))
	{
		choosedColor = color;
	}

	ImGui::Separator();
	ImGui::Text("   Curves list :");
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
	ImGui::Text(" Select a point :");
	ImGui::Text("  right click");
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");
	ImGui::Text("  Move a point");
	ImGui::Text("  Alt + right click");
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");
	ImGui::Text("  Select a curve");
	ImGui::Text("  ctlr + right click");
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");

	//deselectionner
	if (ImGui::Button("Deselect"))
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

	if (ImGui::CollapsingHeader("Bezier curves"))
	{
		// step
		ImGui::Text(" ");
		ImGui::Columns(1);
		ImGui::Text("step :");
		ImGui::SameLine();
		ImGui::Text("%0.1f", step);
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			step = step - 0.5f;
			if (step < 0.00)
				step = 0.00;
		}
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			step = step + 0.5f;
			if (step > 10)
				step = 10;
		}

	
		//fermer la forme
		if (ImGui::Button("Close curve"))
		{
			if (curves.size() > 0)
			{
				for (int i = 0; i < selectedCurves.size(); i++)
				{
					std::vector<Vertex> tempCurve = curves[selectedCurves[i]].getCurvePoints();
					tempCurve.push_back(tempCurve[0]);
					curves[selectedCurves[i]].setCurvePoints(tempCurve);
					curves[selectedCurves[i]].updateBuffers();
				}
			}

			if (vertices.size() > 2)
			{
				vertices.push_back(vertices[0]);
			}
		}

		if (ImGui::Button("Create Bezier curve"))
		{
			if (vertices.size() > 2)
			{
				Curve c(vertices, u, v, iteration, choosedColor);
				curves.push_back(c);
				vertices.clear();
			}
		}

		ImGui::Text("");
		//Racordement
		if (ImGui::CollapsingHeader("Link curves"))
		{
			if (ImGui::Button("Link"))
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
		}

		//Transform
		if (ImGui::CollapsingHeader("Transform"))
		{
			static float translateUI[2] = { 0.0f, 0.0f };
			ImGui::InputFloat2("Translate", translateUI);
			if (ImGui::Button("Translate curve"))
			{
				Matrix t = Matrix::Translate(translateUI[0], translateUI[1]);
				for (int i = 0; i < selectedCurves.size(); i++)
					curves[selectedCurves[i]].Transform(t);
			}

			ImGui::Text("");
			static float rotateUI = 0.0f;
			ImGui::InputFloat("Rotate", &rotateUI);
			if (ImGui::Button("Rotate curve"))
			{
				Matrix t = Matrix::RotateZ(rotateUI);

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
			if (ImGui::Button("Scale curve"))
			{
				Matrix t = Matrix::Scale(scaleUI);
				for (int i = 0; i < selectedCurves.size(); i++)
					curves[selectedCurves[i]].Transform(t);
			}
		}

		//Extrusion
		if (ImGui::CollapsingHeader("Extrusion"))
		{
			ImGui::InputFloat("Height", &extrudeHeight);
			ImGui::InputFloat("Scale ", &extrudeScale);
			ImGui::InputFloat("Step ", &extrudeStep);
			if (ImGui::Button("Extrude"))
			{
				if (selectedCurves.size() != 0)
				{
					sort(selectedCurves.begin(), selectedCurves.end());

					for (int i = selectedCurves.size() - 1; i >= 0; i--)
					{
						meshes.push_back(curves[selectedCurves[i]].SimpleExtrude(extrudeHeight, extrudeScale, extrudeStep));
						curves.erase(curves.begin() + selectedCurves[i]);
					}

					selectedCurves.clear();
				}
			}
		}

		//Revolution
		if (ImGui::CollapsingHeader("Revolution"))
		{
			ImGui::InputFloat("Step", &revolutionStep);
			if (ImGui::Button("Revolution"))
			{
				if (selectedCurves.size() != 0)
				{
					sort(selectedCurves.begin(), selectedCurves.end());

					for (int i = selectedCurves.size() - 1; i >= 0; i--)
					{
						meshes.push_back(curves[selectedCurves[i]].Revolution(revolutionStep));
						curves.erase(curves.begin() + selectedCurves[i]);
					}

					selectedCurves.clear();
				}
			}
		}

		//Generic extrusion 
		if (ImGui::CollapsingHeader("Generic extrusion"))
		{
			if (ImGui::Button("Extrude"))
			{
				if (selectedCurves.size() == 2)
				{
					meshes.push_back(curves[selectedCurves[0]].GenericExtrusion(curves[selectedCurves[1]]));
				}

				selectedCurves.clear();
			}
		}
	}


	if (ImGui::CollapsingHeader("Chaikin curves"))
	{
		// facteur u
		ImGui::Text(" ");
		ImGui::Columns(1);
		ImGui::Text("U factor :");
		ImGui::SameLine();
		ImGui::Text("%.2f", u);
		ImGui::SameLine();
		if (ImGui::Button("-##1"))
		{
			u = u - 0.01f;
			if (u < 0.01)
				u = 0.01;
		}
		ImGui::SameLine();
		if (ImGui::Button("+##1"))
		{
			u = u + 0.01f;
			if (u > 1)
				u = 1;
		}
		//facteur v
		ImGui::Text(" ");
		ImGui::Columns(1);
		ImGui::Text("V factor :");
		ImGui::SameLine();
		ImGui::Text("%.2f", v);
		ImGui::SameLine();
		if (ImGui::Button("-##2"))
		{
			v = v - 0.01f;
			if (v < 0.01)
				v = 0.01;
		}
		ImGui::SameLine();
		if (ImGui::Button("+##2"))
		{
			v = v + 0.01f;
			if (v > 1)
				v = 1;
		}
		//iteration
		ImGui::Text(" ");
		ImGui::Columns(1);
		ImGui::Text("Iteration :");
		ImGui::SameLine();
		ImGui::Text("%.2f", iteration);
		ImGui::SameLine();
		if (ImGui::Button("-##3"))
		{
			iteration = iteration - 1.f;
			if (iteration < 0.f)
				iteration = 0.;
		}
		ImGui::SameLine();
		if (ImGui::Button("+##3"))
		{
			iteration = iteration + 1.f;
			if (iteration > 10.f)
				iteration = 10.f;
		}

		if (ImGui::Button("Close curve"))
		{
			if (curves.size() > 0)
			{
				for (int i = 0; i < selectedCurves.size(); i++)
				{
					std::vector<Vertex> tempCurve = curves[selectedCurves[i]].getCurvePoints();
					tempCurve.push_back(tempCurve[0]);
					curves[selectedCurves[i]].setCurvePoints(tempCurve);
					curves[selectedCurves[i]].updateBuffers();
				}
			}

			if (vertices.size() > 2)
			{
				vertices.push_back(vertices[0]);
			}
		}
		ImGui::Text("");

		if (ImGui::Button("Create chaikin curve"))
		{
			if (vertices.size() > 2)
			{
				Curve c(vertices, u, v, iteration, choosedColor);
				curves.push_back(c);
				vertices.clear();
			}
		}

		if (ImGui::CollapsingHeader("Coons surface"))
		{
			if (ImGui::Button("Create surface"))
			{
#if 0
				std::vector<Vertex> pointsTop = {
					Vertex(-0.5, 0.5, -0.5),
					Vertex(0., 0.5, 0.),
					Vertex(0.5, 0., 0.)
				};

				std::vector<Vertex> pointsDown = {
					Vertex(-0.5, 0., 0.5),
					Vertex(0., -0.5, 0.5),
					Vertex(0.5, 0., 0.5)
				};

				std::vector<Vertex> pointsLeft = {
					Vertex(-0.5, 0., 0.5),
					Vertex(-0.5, 0.5, 0.2),
					Vertex(-0.5, 0.5, -0.5)
				};

				std::vector<Vertex> pointsRight = {
					Vertex(0.5, 0., 0.5),
					Vertex(0., -0.5, 0.2),
					Vertex(0.5, 0., 0.)
				};
#endif
#if 1
				std::vector<Vertex> pointsTop = {
					Vertex(-0.5, 0.5, 0.),
					Vertex(0., 0.5, 0.3),
					Vertex(0.5, 0.5, 0.)
				};

				std::vector<Vertex> pointsDown = {
					Vertex(-0.5, -0.5, 0.),
					Vertex(0., -0.5, 0.3),
					Vertex(0.5, -0.5, 0.)
				};

				std::vector<Vertex> pointsLeft = {
					Vertex(-0.5, -0.5, 0.),
					Vertex(-0.5, 0., 0.3),
					Vertex(-0.5, 0.5, 0.)
				};

				std::vector<Vertex> pointsRight = {
					Vertex(0.5, -0.5, 0.),
					Vertex(0.5, 0., 0.3),
					Vertex(0.5, 0.5, 0.)
				};
#endif

				curves.push_back(Curve(pointsTop, u, v, iteration, Color(0, 0, 0)));
				curves.push_back(Curve(pointsDown, u, v, iteration, Color(0, 0, 0)));
				curves.push_back(Curve(pointsLeft, u, v, iteration, Color(0, 0, 0)));
				curves.push_back(Curve(pointsRight, u, v, iteration, Color(0, 0, 0)));

				meshes.push_back(generateCoon(curves, u, v));
			}

			ImGui::Text(" ");
			ImGui::Columns(1);
			ImGui::Text("Etape coons :");
			ImGui::SameLine();
			ImGui::Text("%d", etape);
			ImGui::SameLine();
			if (ImGui::Button("-##4"))
			{
				etape = etape - 1;
				if (iteration < 0)
					iteration = 0;
				meshes.clear();
				meshes.push_back(generateCoon(curves, u, v));
			}
			ImGui::SameLine();
			if (ImGui::Button("+##4"))
			{
				etape = etape + 1;
				if (etape > 3)
					etape = 3;
				meshes.clear();
				meshes.push_back(generateCoon(curves, u, v));
			}
		}
	}

	if (ImGui::CollapsingHeader("Subdivision"))
	{
		ImGui::Text("         With quad mesh   ");
		if (ImGui::Button("Create quad cube"))
		{
			faces = GenerateCubeFaces();
		}
		if (ImGui::Button("Doo-Sabin (D)"))
		{
			faces = DooSabin(faces);
		}
		if (ImGui::Button("CatmulClark (C)"))
		{
			faces = CatmullClark(faces);
		}

		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("        With triangulate mesh   ");

		if (ImGui::Button("Create tris cube"))
		{
			faces = GenerateTriFaces();
		}

		if (ImGui::Button("Loop (L)"))
		{
			faces = LoopAlgo(faces);
		}

		if (ImGui::Button("Kobelt (K)"))
		{
			faces = Kobelt(faces);
		}

		ImGui::Separator();
		ImGui::Text(" Delete mesh with backspace key  ");
	}

	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("            Visualizer    ");
	ImGui::Text("");

	if (ImGui::Button("Wireframe (W)"))
	{
		enableWireframe = !enableWireframe;
	}
	ImGui::SameLine();
	if (ImGui::Button("Normales (N)"))
	{
		enableNormal = !enableNormal;
	}
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("");
	//3D viewport 
	if (ImGui::Button("3D Viewport (V)"))
	{
		enable3DViewport = !enable3DViewport;
		updateVBO();
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

