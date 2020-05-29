#pragma once
#include "Mesh.h"
#include "OpenGLcore.h"

Mesh::Mesh()
{
	vertices = {};
	indices = {};
	normals = {};
	uvs = {};

	VBO = 0;
}

void Mesh::updateBuffers()
{
	VBO = CreateBufferObject(BufferType::VBO, sizeof(Vertex) * vertices.size(), vertices.data());
	IBO = CreateBufferObject(BufferType::IBO, sizeof(short) * indices.size(), indices.data());
}
