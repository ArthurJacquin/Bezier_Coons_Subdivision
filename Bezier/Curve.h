#pragma once
#include <vector>
#include "Vertex.h"

using namespace std;

class Curve
{
	vector<Vertex> controlPoints;
	vector<Vertex> curvePoints;
	float step;
	uint32_t VBOControl;
	uint32_t VBOCurve;

public:
	Curve(vector<Vertex> controlPoints, vector<Vertex> curvePoints);
	Curve(vector<Vertex> controlPoints, float step);

	void createBeziers(std::vector<Vertex>& curvePoints, std::vector<Vertex> controlPoints, float step);

	vector<Vertex> getControlPoints() { return controlPoints; }
	vector<Vertex> getCurvePoints() { return curvePoints; }
	uint32_t getVBOControl() { return VBOControl; }
	uint32_t getVBOCurve() { return VBOCurve; }
};

