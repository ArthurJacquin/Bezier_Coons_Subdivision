#include "Face.h"
#include "OpenGLcore.h"

Face::Face()
{
	vertices = {};
	indices = {};
	edges = {};

	VBO = 0;
}

Face::Face(vector<Vertex> verts, Color col, Face* p)
	: parent(p)
{
	indices = {
		0, 1, 2,
		0, 2, 3
	};
	
	if (p == nullptr) 
	{
		vertices = verts;

		edges = {
			Edge(verts[0], verts[1]),
			Edge(verts[1], verts[2]),
			Edge(verts[2], verts[3]),
			Edge(verts[3], verts[0])
		};
	}
	else 
	{
		vertices = verts;
		for (size_t i = 0; i < verts.size(); i++)
		{
			vertices[i].parent = &p->vertices[i];
		}

		edges = {
			Edge(verts[0], verts[1], &p->edges[0]),
			Edge(verts[1], verts[2], &p->edges[1]),
			Edge(verts[2], verts[3], &p->edges[2]),
			Edge(verts[3], verts[0], &p->edges[3])
		};
	}

	CalculateNormals();
	SetColor(col);
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

void Face::SetColor(Color col)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].setColor(col);
	}
}
