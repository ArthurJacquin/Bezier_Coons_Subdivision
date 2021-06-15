#include "Face.h"
#include "OpenGLcore.h"

Face::Face()
{
	vertices = {};
	indices = {};
	edges = {};

	VBO = 0;
}

Face::Face(vector<Vertex*> verts, Color col, Face* p)
	: parent(p)
{
	if (verts.size() == 3)
	{
		indices = {
			0, 1, 2,
		};
	}
	else {
		indices = {
			0, 1, 2,
			0, 2, 3
		};
	}

	if (p == nullptr) 
	{
		vertices = verts;

		for (size_t i = 0; i < verts.size(); i++)
		{
			edges.push_back(new Edge(verts[i], verts[(i + 1) % verts.size()]));
		}
	}
	else 
	{
		vertices = verts;
		for (size_t i = 0; i < verts.size(); i++)
		{
			vertices[i]->parent = p->vertices[i];
		}

		for (size_t i = 0; i < verts.size(); i++)
		{
			edges.push_back(new Edge(verts[i], verts[(i + 1) % verts.size()], p->edges[i]));
		}
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

				//Vertex p1 = *vertices[indices[j]];
				//Vertex p2 = *vertices[indices[j + 1]];
				//Vertex p3 = *vertices[indices[j + 2]];

				Vec3 v1(vertices[indices[j + 1]]->x - vertices[indices[j]]->x, vertices[indices[j + 1]]->y - vertices[indices[j]]->y, vertices[indices[j + 1]]->z - vertices[indices[j]]->z);
				Vec3 v2(vertices[indices[j + 2]]->x - vertices[indices[j]]->x, vertices[indices[j + 2]]->y - vertices[indices[j]]->y, vertices[indices[j + 2]]->z - vertices[indices[j]]->z);

				sum += (v1 ^ v2).normalise();
			}
		}

		vertices[i]->normal = (sum / shared).normalise();
	}
}

void Face::updateBuffers()
{
	updateBufferPoints();
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * bufferPts.size(), bufferPts.data());
}

void Face::SetColor(Color col)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i]->setColor(col);
	}
}

void Face::updateBufferPoints()
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		bufferPts.push_back(*vertices[i]);
	}
}