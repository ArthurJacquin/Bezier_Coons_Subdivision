#include "MeshGeneration.h"

std::vector<Face> GenerateCubeFaces()
{
	//Front
	Vertex* p0 = new Vertex(-0.5f, -0.5f, 0.0f);
	Vertex* p1 = new Vertex(0.5f, -0.5f, 0.0f);
	Vertex* p2 = new Vertex(0.5f, 0.5f, 0.0f);
	Vertex* p3 = new Vertex(-0.5f, 0.5f, 0.0f);

	//Back   
	Vertex* p4 = new Vertex(-0.5f, -0.5f, 1.0f);
	Vertex* p5 = new Vertex(0.5f, -0.5f, 1.0f);
	Vertex* p6 = new Vertex(0.5f, 0.5f, 1.0f);
	Vertex* p7 = new Vertex(-0.5f, 0.5f, 1.0f);

	return {

		Face({p0, p1, p2, p3}), //Front
		Face({p5, p4, p7, p6}), //Back
		Face({p3, p2, p6, p7}),	//Top
		Face({p4, p5, p1, p0}),	//Bottom
		Face({p4, p0, p3, p7}),	//Left
		Face({p1, p5, p6, p2}),	//Right
	};
}

std::vector<Face> GenerateTriFaces()
{
	//Tri 1
	Vertex* p0 = new Vertex(-0.5f, -0.5f, 0.0f);
	Vertex* p1 = new Vertex(0.5f, -0.5f, 0.0f);
	Vertex* p2 = new Vertex(0.5f, 0.5f, 0.0f);

	Vertex* p3 = new Vertex(-0.5f, 0.5f, 0.0f);  
	Vertex* p4 = new Vertex(-0.5f, -0.5f, 1.0f);
	Vertex* p5 = new Vertex(0.5f, -0.5f, 1.0f);

	Vertex* p6 = new Vertex(0.5f, 0.5f, 1.0f);
	Vertex* p7 = new Vertex(-0.5f, 0.5f, 1.0f);

	return {

		Face({p0, p1, p3}), //Front
		Face({p1, p2, p3}),
		Face({p5, p4, p6}), //Back
		Face({p4, p7, p6}),
		Face({p3, p2, p7}),	//Top
		Face({p2, p6, p7}),	
		Face({p0, p1, p4}),	//Bottom
		Face({p1, p5, p4}),
		Face({p0, p3, p4}),	//Left
		Face({p3, p7, p4}),	
		Face({p1, p5, p2}),	//Right
		Face({p5, p6, p2}),
	};
}
