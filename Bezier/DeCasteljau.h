#pragma once
#include <vector>
#include "Vertex.h"

std::vector<Vertex> createBeziers(std::vector<Vertex> tabControlPoints, float step)
{
	std::vector<Vertex> tabBezierPoints;
	for (int t = 0; t < 1; t + step)
	{
		for (int j = 1; j < tabControlPoints.size(); j++)
		{
			for (int i = 0; i < tabControlPoints.size() - j; i++)
			{
				//tabBezierPoints[] = (1 - t) * tab;
			}
		}


	}

	return tabBezierPoints;
}
