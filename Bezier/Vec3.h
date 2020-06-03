#pragma once
#include <math.h>
#include <iostream>


class Vec3 {

public:

	union {
		float data[3];
		struct {
			float x;
			float y;
			float z;
		};
	};

	// Constructors

	// Vectors default to 0, 0, 0.
	Vec3() {
		x = 0;
		y = 0;
		z = 0;
	}

	// Construct with values, 3D
	Vec3(float ax, float ay, float az) {
		x = ax;
		y = ay;
		z = az;
	}

	// Construct with values, 2D
	Vec3(float ax, float ay) {
		x = ax;
		y = ay;
		z = 0;
	}

	// Copy constructor
	Vec3(const Vec3& o) {
		x = o.x;
		y = o.y;
		z = o.z;
	}

	//Equality
	bool operator==(const Vec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	// Addition
	Vec3 operator+(const Vec3& o) {
		return Vec3(x + o.x, y + o.y, z + o.z);
	}

	Vec3& operator+=(const Vec3& o) {
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	// Subtraction

	Vec3 operator-() {
		return Vec3(-x, -y, -z);
	}

	Vec3 operator-(const Vec3 o) {
		return Vec3(x - o.x, y - o.y, z - o.z);
	}

	Vec3& operator-=(const Vec3 o) {
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	// Multiplication by scalars

	Vec3 operator*(const float s) {
		return Vec3(x * s, y * s, z * s);
	}

	Vec3& operator*=(const float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	// Division by scalars

	Vec3 operator/(const float s) {
		return Vec3(x / s, y / s, z / s);
	}

	Vec3& operator/=(const float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	// Dot product

	float operator*(const Vec3 o) {
		return (x * o.x) + (y * o.y) + (z * o.z);
	}

	// An in-place dot product does not exist because
	// the result is not a vector.

	// Cross product

	Vec3 operator^(const Vec3 o) {
		float nx = y * o.z - o.y * z;
		float ny = z * o.x - o.z * x;
		float nz = x * o.y - o.x * y;
		return Vec3(nx, ny, nz);
	}

	Vec3& operator^=(const Vec3 o) {
		float nx = y * o.z - o.y * z;
		float ny = z * o.x - o.z * x;
		float nz = x * o.y - o.x * y;
		x = nx;
		y = ny;
		z = nz;
		return *this;
	}

	// Other functions

	// Length of vector
	float magnitude() {
		return sqrt(magnitude_sqr());
	}

	// Length of vector squared
	float magnitude_sqr() {
		return (x * x) + (y * y) + (z * z);
	}

	// Modified the vector so it becomes normalised
	Vec3& normalise() {
		float invLength = 1 / sqrtf(x * x + y * y + z * z);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		return *this;
	}

	float dot(Vec3 b) {
		return (*this).x * b.x + (*this).y * b.y + (*this).z * b.z;
	}
};
