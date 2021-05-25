#pragma once
#include "Mesh.h"
#include "Curve.h"

Mesh generateCoon(std::vector<Curve> curves, float u, float v);
void generateRuledSurface(std::vector<Vertex>& ruledSurface, Curve c1, Curve c2, float resolution);
