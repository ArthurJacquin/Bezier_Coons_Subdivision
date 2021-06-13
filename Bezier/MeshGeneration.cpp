#include "MeshGeneration.h"

Mesh GenerateCube()
{
	std::vector<Vertex> vertices = {

		//Front
		Vertex(-0.5f, -0.5f, -0.5f), 
		Vertex(0.5f, -0.5f, -0.5f),
		Vertex(-0.5f, 0.5f, -0.5f),
		Vertex(0.5f, 0.5f, -0.5f),

		//Back
		Vertex(-0.5f, -0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, 0.5f, 0.5f),
		Vertex(0.5f, 0.5f, 0.5f),
	};

	std::vector<uint32_t> indices = {

		//Front
		0, 1, 3,
		0, 3, 2,

		//Top
		2, 3, 7,
		2, 7, 6,

		//Back
		6, 7, 5,
		6, 5, 4,

		//Bottom
		4, 5, 0,
		0, 1, 5,

		//Left
		4, 2, 6,
		4, 0, 2,

		//Right
		3, 1, 7,
		7, 1, 5

	};

	return Mesh(vertices, indices);
}