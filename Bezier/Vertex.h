#pragma once
#include <iostream>
#include "Color.h"
#include "Vec3.h"
#include <vector>

struct Vertex
{
	double x, y, z;
	float r, g, b;
	Vec3 normal;
	std::vector<Vertex*> neighborVertices;

	Vertex();
	Vertex(Color col);
	Vertex(double x, double y, double z, float r, float g, float b);
	Vertex(double x, double y, double z);

	friend std::ostream& operator <<(std::ostream&, const Vertex& obj);
	bool operator!=(const Vertex v) { return x != v.x || y != v.y || z != v.z || r != v.r || g != v.g || b != v.b; }
	bool operator==(const Vertex v) { return !(*this != v); }
	Vertex& operator=(Vertex v) { x = v.x; y = v.y; z = v.z; r = v.r; g = v.g; b = v.b; return *this; }
	Vertex& operator*(int i) { x = x * i; y = y * i; z = z * i; return *this; }
	Vertex operator+(const Vec3& v) const { return Vertex(x + v.x, y + v.y, z + v.z); }
	Vertex operator-(const Vec3& v) const { return Vertex(x - v.x, y - v.y, z - v.z); }
	
	friend bool operator<(const Vertex v, const Vertex v2) { if (v.x == v2.x) return v.y < v2.y; else return v.x < v2.x; }

	void setColor(Color color);
	void setPositionUsingMouse(double x, double y);
	void setNormal(Vec3 n);
	double Distance(Vertex v2) { return sqrt(pow((v2.x - x), 2) + pow((v2.y - y), 2) + pow((v2.z - z), 2)); }

	Vec3 GetPos()const { return Vec3(x, y, z); }
	Color GetColor()const { return Color(r, g, b); }
	std::vector<Vertex*> getNeighborVertices() const { return neighborVertices; }
	void addNeighborVertices(Vertex* v) { neighborVertices.push_back(v); }
};