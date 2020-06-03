#include<algorithm>
#include "Curve.h"
#include "OpenGLcore.h"
#include "Color.h"

#define M_PI 3.14159265

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

void Curve::updateBuffers()
{
	VBOControl = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * controlPoints.size(), controlPoints.data());
	VBOCurve = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * curvePoints.size(), curvePoints.data());
}

void Curve::link(Curve& c2)
{
	std::vector<Vertex>* cp1 = &this->controlPoints;

	std::vector<Vertex> newControlPoints;

	std::copy_n(cp1->begin(), cp1->size() - 1, std::back_inserter(newControlPoints));

	Vertex meanPoint(Color(1.0f, 1.0f, 1.0f));
	meanPoint.x = (cp1->back().x + c2.controlPoints[0].x) / 2;
	meanPoint.y = (cp1->back().y + c2.controlPoints[0].y) / 2;
	newControlPoints.push_back(meanPoint);

	std::copy_n(c2.controlPoints.begin() + 1, c2.controlPoints.size() - 1, std::back_inserter(newControlPoints));

	c2.clearControlPoints();
	c2.clearCurvePoints();

	this->controlPoints = newControlPoints;
	updateCurve();
}

void Curve::Transform(Matrix& mat)
{
	for (int i = 0; i < controlPoints.size(); i++)
	{
		controlPoints[i] = mat * controlPoints[i];
	}

	updateCurve();
}

void Curve::setCurveColor(Color col)
{
	for (int i = 0; i < curvePoints.size(); ++i)
	{
		curvePoints[i].setColor(col);
	}

	color = col;
	updateBuffers();
}

void Curve::setControlPoints(vector<Vertex> v)
{
	controlPoints.resize(v.size());
	for (int i = 0; i < controlPoints.size(); i++)
	{
		controlPoints[i] = v[i];
	}
}

void Curve::setControlPoints(int id, Vertex v)
{
	controlPoints[id] = v;
}

void Curve::setControlPointColor(int id, Color col)
{
	Vertex v = getControlPoints()[id];
	v.setColor(col);
	setControlPoints(id, v);
	updateBuffers();
}

void Curve::addControlPointAtIndex(int id, Vertex v)
{
	controlPoints.insert(controlPoints.begin() + id, v);
	updateCurve();
}

Mesh Curve::SimpleExtrude(int h, float scale, float step)
{
	Mesh m;
	vector<Vertex> curvePoints = this->getCurvePoints();

	bool close = false;
	if(close)
		curvePoints.push_back(curvePoints[0]);

	for (float t = 0; t < 1; t += step)
	{
		for (int s = 0; s < curvePoints.size(); s++)
		{
			float x = curvePoints[s].x * (1 + t * (scale - 1));
			float y = curvePoints[s].y * (1 + t * (scale - 1));
			float z = h * t;

			m.getVertices().push_back(Vertex(x, y, z));
		}
	}
	
	m.CalculateIndices(curvePoints.size(), 1 / step - 1);
	m.CalculateNormals();
	m.updateBuffers();

	return m;
}

Mesh Curve::Revolution(float step)
{
	Mesh m;
	vector<Vertex> curvePoints = this->getCurvePoints();

	for (float t = 0; t < 1; t += step)
	{
		for (int s = 0; s < curvePoints.size(); s++)
		{
			float rad = 360 * t * M_PI / 180.0f;

			float x = curvePoints[s].x * cos(rad);
			float y = curvePoints[s].y;
			float z = curvePoints[s].x * cos(rad);

			m.getVertices().push_back(Vertex(x, y, z));
		}
	}

	m.CalculateIndices(curvePoints.size(), 1 / step - 1);
	m.CalculateNormals();
	m.updateBuffers();

	return m;
}


