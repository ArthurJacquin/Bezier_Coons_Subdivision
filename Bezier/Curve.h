#pragma once
#include <vector>
#include "Vertex.h"
#include "Matrix.h"
#include "Mesh.h"

using namespace std;

class Curve
{
	vector<Vertex> controlPoints;
	vector<Vertex> curvePoints;
	float step;
	uint32_t VBOControl;
	uint32_t VBOCurve;
	Color color;

public:
	Curve();
	Curve(vector<Vertex> controlPoints, vector<Vertex> curvePoints, Color col);
	Curve(vector<Vertex> controlPoints, float step, Color color);

	void createBeziers(std::vector<Vertex>& curvePoints, std::vector<Vertex> controlPoints, float step, Color choosedColor);
	void updateCurve();
	void updateBuffers();
	void link(Curve& c2);

	//Matrix
	void Transform(Matrix& mat);

	//Curve point functions
	void clearCurvePoints() { curvePoints.clear(); }
	void setCurveColor(Color col);

	//Control points functions
	void clearControlPoints() { controlPoints.clear(); }
	void setControlPoints(vector<Vertex> v);
	void setControlPoints(int id, Vertex v);
	void setControlPointColor(int id, Color col);
	void addControlPointAtIndex(int id, Vertex v);

	void setCurvePoints(std::vector<Vertex> v);

	vector<Vertex> getControlPoints()const { return controlPoints; }
	vector<Vertex> getCurvePoints()const { return curvePoints; }
	uint32_t getVBOControl()const { return VBOControl; }
	uint32_t getVBOCurve()const { return VBOCurve; }
	Color getColor() const { return color; }

	//Extrusion
	Mesh SimpleExtrude(int h, float scale, float step);
	Mesh Revolution(float step);
	Mesh GenericExtrusion(Curve& path, float step);

};

