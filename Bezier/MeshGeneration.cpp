#include "MeshGeneration.h"

std::vector<Face> GenerateCubeFaces()
{
	//Front
	Vertex p0(-0.5f, -0.5f, 0.0f);
	Vertex p1(0.5f, -0.5f, 0.0f);
	Vertex p2(0.5f, 0.5f, 0.0f);
	Vertex p3(-0.5f, 0.5f, 0.0f);

	//Back
	Vertex p4(-0.5f, -0.5f, 1.0f);
	Vertex p5(0.5f, -0.5f, 1.0f);
	Vertex p6(0.5f, 0.5f, 1.0f);
	Vertex p7(-0.5f, 0.5f, 1.0f);

	return {

		Face({p0, p1, p2, p3}), //Front
		Face({p5, p4, p7, p6}), //Back
		Face({p3, p2, p6, p7}),	//Top
		Face({p4, p5, p1, p0}),	//Bottom
		Face({p4, p0, p3, p7}),	//Left
		Face({p1, p5, p6, p2}),	//Right
	};
}