#pragma once
#include <iostream>
#include "Color.h"
#include "Vec3.h"
#include <vector>

#define EPSI  0.01F

struct Vertex
{
	double x, y, z;
	float r, g, b;
	Vec3 normal;
	Vertex* parent;
	//Catmull clark
	Vertex* vPrime;

	Vertex();
	Vertex(Color col);
	Vertex(double x, double y, double z, float r, float g, float b);
	Vertex(double x, double y, double z);
	Vertex(Vec3 pos);
	Vertex(Vec3 pos, Color col, Vertex* parent = nullptr);

	friend std::ostream& operator <<(std::ostream&, const Vertex& obj);
	bool operator!=(const Vertex v) const {
		return abs(x - v.x) > EPSI || abs(y - v.y) > EPSI || abs(z - v.z) > EPSI;
	}
	bool operator==(const Vertex v) const { return !(*this != v); }
	Vertex& operator=(Vertex v) { x = v.x; y = v.y; z = v.z; r = v.r; g = v.g; b = v.b; normal = v.normal; parent = v.parent; vPrime = v.vPrime; return *this; }
	Vertex& operator+=(const Vertex& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vertex& operator/=(const float v) { x /= v; y /= v; z /= v; return *this; }
	Vertex& operator*(int i) { x = x * i; y = y * i; z = z * i; return *this; }
	Vertex& operator/(float i) { x = x / i; y = y / i; z = z / i; return *this; }
	Vertex operator+(const Vec3& v) const { return Vertex(x + v.x, y + v.y, z + v.z); }
	Vertex operator+(const Vertex& v) const { return Vertex(x + v.x, y + v.y, z + v.z); }
	Vertex operator-(const Vec3& v) const { return Vertex(x - v.x, y - v.y, z - v.z); }
	
	friend bool operator<(const Vertex v, const Vertex v2) { if (v.x == v2.x) return v.y < v2.y; else return v.x < v2.x; }

	void setColor(Color color);
	void setPositionUsingMouse(double x, double y);
	void setNormal(Vec3 n);
	double Distance(Vertex v2) { return sqrt(pow((v2.x - x), 2) + pow((v2.y - y), 2) + pow((v2.z - z), 2)); }

	Vec3 GetPos()const { return Vec3(x, y, z); }
	Color GetColor()const { return Color(r, g, b); }
	bool HaveTheSameParent(const Vertex& v);
};