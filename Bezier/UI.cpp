#pragma once
#include "UI.h"

UI::UI(int width, int height)
{
	tabMenuForme = new float[4] {
		0, -1.0f + 2 * 100 / width,
		-1.0f + 2 * 100 / height, -1.0f + 2 * 100 / width
	};
}
