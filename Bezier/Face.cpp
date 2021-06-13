#include "Face.h"
#include "OpenGLcore.h"

Face::Face()
{
	vertices = {};
	indices = {};
	edges = {};

	VBO = 0;
}

Face::Face(vector<Vertex> verts)
	: vertices(verts)
{
	indices = {
		0, 3, 2,
		0, 1, 3
	};
	
	edges = {
		make_pair(verts[0], verts[1]),
		make_pair(verts[1], verts[3]),
		make_pair(verts[3], verts[2]),
		make_pair(verts[2], verts[0]),
	};

	CalculateNormals();
	updateBuffers();
}

void Face::CalculateNormals()
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

void Face::updateBuffers()
{
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * vertices.size(), vertices.data());
}