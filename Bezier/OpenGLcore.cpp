#include "OpenGLcore.h"

uint32_t CreateBufferObject(BufferType type, const size_t size, const void* data)
{
	uint32_t BO;
	glGenBuffers(1, &BO);
	GLenum target;
	switch (type)
	{
	case BufferType::IBO:
		target = GL_ELEMENT_ARRAY_BUFFER;
		break;
	default:
	case BufferType::VBO:
		target = GL_ARRAY_BUFFER;
		break;
	}
	glBindBuffer(target, BO);
	glBufferData(target, size, data, GL_STATIC_DRAW);
	return BO;
}

void DeleteBufferObject(uint32_t& BO) {
	glDeleteBuffers(1, &BO);
	BO = 0;
}