#pragma once

#include <cstdint>

#define GLEW_STATIC
#include <GL/glew.h>

enum BufferType
{
	VBO,
	IBO,
	MAX
};

uint32_t CreateBufferObject(BufferType type, const size_t size, const void* data);

void DeleteBufferObject(uint32_t& BO);