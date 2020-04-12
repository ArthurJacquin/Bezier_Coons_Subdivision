#include "Curve.h"
#include "OpenGLcore.h"
#include "Color.h"

Curve::Curve()
{
	controlPoints = {};
	curvePoints = {};
	step = 0;
	VBOControl = 0;
	VBOCurve = 0;
	color = Color(1.0f, 0.0f, 0.0f);
}

Curve::Curve(vector<Vertex> controlPoints, vector<Vertex> curvePoints, Color color)
: controlPoints(controlPoints), curvePoints(curvePoints), step(0.1f), color(color)
{
	VBOControl = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * controlPoints.size(), controlPoints.data());
	VBOCurve = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * curvePoints.size(), curvePoints.data());
}

Curve::Curve(vector<Vertex> controlPoints, float step, Color color)
: controlPoints(controlPoints), step(step), color(color)
{
	createBeziers(curvePoints, controlPoints, step, color);
	VBOControl = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * controlPoints.size(), controlPoints.data());
	VBOCurve = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * curvePoints.size(), curvePoints.data());
}

void Curve::createBeziers(std::vector<Vertex>& tabBezierPoints, std::vector<Vertex> tabControlPoints, float step, Color choosedColor)
{
	Vertex temp = tabControlPoints[0];
	temp.setColor(choosedColor);
	tabBezierPoints.push_back(temp);

	//teste si il y a plus d'un point de controle
	if (tabControlPoints.size() > 2)
	{
		std::vector<Vertex> barycentre;

		for (float t = 0; t < 1; t += step)
		{
			//reset des barycentres
			barycentre = tabControlPoints;

			for (int j = 1; j < tabControlPoints.size(); j++)
			{
				for (int i = 0; i < tabControlPoints.size() - j; i++)
				{
					barycentre[i].x = (1 - t) * barycentre[i].x + t * barycentre[i + 1].x;
					barycentre[i].y = (1 - t) * barycentre[i].y + t * barycentre[i + 1].y;
				}
			}

			barycentre[0].setColor(choosedColor);
			tabBezierPoints.push_back(barycentre[0]);
		}
	}

	temp = tabControlPoints.back();
	temp.setColor(choosedColor);
	tabBezierPoints.push_back(temp);
}

void Curve::updateCurve()
{
	Curve newCurve(this->controlPoints, this->step, this->color);
	*this = newCurve;
}

void Curve::setControlPoints(vector<Vertex> v)
{
	controlPoints.resize(v.size());
	for (int i = 0; i < controlPoints.size(); i++)
	{
		controlPoints[i] = v[i];
	}
}
