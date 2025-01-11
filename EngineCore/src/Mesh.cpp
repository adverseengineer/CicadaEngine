
#include "Mesh.h"

#include <glad/glad.h>

#define NOMINMAX
#include <OBJ_Loader.h>

Mesh::Mesh(unsigned int numElemsPerVert) : m_numElemsPerVert(numElemsPerVert) {
	m_primitiveType = GL_TRIANGLES;
	glGenVertexArrays(1, &m_vaoId);
	glGenBuffers(1, &m_vboId);
	glGenBuffers(1, &m_iboId);

	//these are the default always there implicit attributes. others can be explicitly added
	AddVertexAttribute("position", 0, 3, 0);
	AddVertexAttribute("vertexColor", 1, 4, 3);
	AddVertexAttribute("vertexNormal", 2, 3, 7);
	AddVertexAttribute("texCoord", 3, 2, 10);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_vaoId);
	glDeleteBuffers(1, &m_vboId);
	glDeleteBuffers(1, &m_iboId);
}

void Mesh::Bind() const {
	glBindVertexArray(m_vaoId);
}

void Mesh::Unbind() const {
	glBindVertexArray(0);
}

void Mesh::Setup() const {

	glBindVertexArray(m_vaoId); //select this mesh's vao for modification
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId); //tie this mesh's vbo into the vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId); //tie this mesh's ibo into the vao

	//set up vertex attribute pointers
	for (const auto& item : attributeMap) {
		const VertexAttribute& attr = item.second;
		glEnableVertexAttribArray(attr.m_index);
		glVertexAttribPointer(
			attr.m_index,
			attr.m_numComponents,
			attr.m_type,
			attr.m_isNormalized,
			attr.m_bytesToNext,
			attr.m_byteOffset
		);
	}

	//unselect this vao for safety
	glBindVertexArray(0);
}

bool Mesh::LoadObj(const std::string& objPath) {

	objl::Loader loader;
	if (!loader.LoadFile(objPath))
		return false;

	for (const auto& elem : loader.LoadedMeshes[0].Vertices) {
		AddVertexData(
			12,
			elem.Position.X, elem.Position.Y, elem.Position.Z,
			1.0f, 1.0f, 1.0f, 1.0f,
			elem.Normal.X, elem.Normal.Y, elem.Normal.Z,
			elem.TextureCoordinate.X, elem.TextureCoordinate.Y
		);
	}

	for (const auto& elem : loader.LoadedMeshes[0].Indices) {
		AddIndexData(1, elem);
	}

	return true;
}

void Mesh::Upload() const {
	glBindVertexArray(m_vaoId);
	unsigned long long vboBytes = m_vertexData.size() * sizeof(float);
	unsigned long long iboBytes = m_indexData.size() * sizeof(unsigned short);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vboBytes, (void*)m_vertexData.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)iboBytes, (void*)m_indexData.data(), GL_STATIC_DRAW);
}

void Mesh::AddVertexAttribute(const std::string& name, unsigned int index, unsigned int numberOfElements, unsigned int offsetCount) {
	unsigned int vertexSizeInBytes = sizeof(float) * m_numElemsPerVert;
	unsigned int bytesToNext = vertexSizeInBytes;
	unsigned long long offsetBytes = sizeof(float) * offsetCount;
	attributeMap[name] = {
		index,
		numberOfElements,
		GL_FLOAT,
		GL_FALSE,
		bytesToNext,
		(void*)offsetBytes
	};
}

void Mesh::AddVertexData(unsigned int count, ...) {
	if (count != m_numElemsPerVert)
		throw "Invalid vertex data count!";

	va_list args;
	va_start(args, count);
	while (count > 0) {
		//the default is double, so accept as double and then cast to float
		m_vertexData.push_back(static_cast<float>(va_arg(args, double)));
		count--;
	}
	va_end(args);
}

void Mesh::AddIndexData(unsigned int count, ...) {

	va_list args;
	va_start(args, count);
	while (count-- > 0) //the default is double, so accept as double and then cast to float
		m_indexData.push_back(static_cast<unsigned short>(va_arg(args, int)));

	va_end(args);
}
