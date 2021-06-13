#include "MeshGeneration.h"

std::vector<Face> GenerateCubeFaces()
{

	//Front
	Vertex p0(-0.5f, -0.5f, -0.5f);
	Vertex p1(0.5f, -0.5f, -0.5f);
	Vertex p2(-0.5f, 0.5f, -0.5f);
	Vertex p3(0.5f, 0.5f, -0.5f);

	//Back
	Vertex p4(-0.5f, -0.5f, 0.5f);
	Vertex p5(0.5f, -0.5f, 0.5f);
	Vertex p6(-0.5f, 0.5f, 0.5f);
	Vertex p7(0.5f, 0.5f, 0.5f);

	return {

		Face({p0, p1, p2, p3}), //Front
		Face({p4, p5, p6, p7}), //Back
		Face({p2, p3, p6, p7}),	//Top
		Face({p4, p5, p0, p1}),	//Bottom
		Face({p6, p2, p4, p0}),	//Left
		Face({p3, p7, p1, p5}),	//Right
	};
}