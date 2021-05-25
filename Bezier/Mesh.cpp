#pragma once
#include "Mesh.h"
#include "OpenGLcore.h"

Mesh::Mesh()
{
	vertices = {};
	indices = {};

	VBO = 0;
}

Mesh::Mesh(std::vector<Vertex> v, int r, int c)
{
	vertices = v;
	CalculateIndices(r, c);
	//CalculateNormals();
	updateBuffers();
}

void Mesh::CalculateIndices(int rows, int columns)
{	
	int nbIndices = (rows * 2 - 2) * columns * 3;

	indices.resize(nbIndices);

	int a = 0;
	int b = rows;
	int nb = 0;
	int cc = 0;

	for (int i = 0; i < nbIndices; i += 3)
	{
		if (i % 2 == 0)
		{
			indices[i] = a;
			indices[i + 1] = b;
			indices[i + 2] = a + 1;

			a++;
		}
		else
		{
			indices[i] = a;
			indices[i + 1] = b;
			indices[i + 2] = b + 1;

			b++;
		}

		nb ++;

		if (nb == rows * 2 - 2)
		{
			cc++;
			a = rows * cc;
			b = rows * (cc + 1);
			nb = 0;
		}
	}
}

void Mesh::CalculateNormals()
{
	int shared;
	Vec3 sum;

	for (int i = 0; i < vertices.size(); i++)
	{
		shared = 0;
		sum = Vec3();

		for (int j = 0; j < indices.size(); j += 3)
		{
			if (i == indices[j] || i == indices[j + 1] || i == indices[j + 2])
			{
				shared++;

				Vertex p1 = vertices[indices[j]];
				Vertex p2 = vertices[indices[j + 1]];
				Vertex p3 = vertices[indices[j + 2]];

				Vec3 v1(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
				Vec3 v2(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

				sum += (v1 ^ v2).normalise();
			}
		}

		vertices[i].normal = (sum / shared).normalise();
	}
}

void Mesh::updateBuffers()
{
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * vertices.size(), vertices.data());
}

void Mesh::Transform(Matrix m)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = m * vertices[i];
	}
}
