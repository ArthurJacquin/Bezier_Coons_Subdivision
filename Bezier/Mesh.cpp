#pragma once
#include "Mesh.h"
#include "OpenGLcore.h"

Mesh::Mesh()
{
	vertices = {};
	indices = {};
	normals = {};
	uvs = {};

	VBO = 0;
}

void Mesh::CalculateIndices(int rows, int columns)
{
	int nbIndices;
	if (columns > 1)
		nbIndices = rows * 2 * (columns - 1) + (columns - 2) * 2;
	else
		nbIndices = rows * 2;

	indices.resize(nbIndices + 2);

	int a = 0;
	int b = rows;
	int nb = 0;

	for (int i = 0; i < nbIndices; i += 2)
	{
		indices[i] = a;
		indices[i + 1] = b;
		nb++;

		if (nb == rows)
		{
			indices[i + 2] = b;
			indices[i + 3] = a + 1;
			i += 2;
			nb = 0;
		}

		a++;
		b++;
	}

	indices.pop_back();
}

void Mesh::updateBuffers()
{
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * vertices.size(), vertices.data());
	IBO = CreateBufferObject(BufferType::IBO, sizeof(short) * indices.size(), indices.data());
}
