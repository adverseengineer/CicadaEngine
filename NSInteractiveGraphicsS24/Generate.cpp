#include "Generate.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define NORM_UP 0.0f, 1.0f, 0.0f
#define NORM_DOWN 0.0f, -1.0f, 0.0f
#define NORM_LEFT -1.0f, 0.0f, 0.0f
#define NORM_RIGHT 1.0f, 0.0f, 0.0f
#define NORM_FORWARD 0.0f, 0.0f, -1.0f
#define NORM_BACKWARD 0.0f, 0.0f, 1.0f

/*
std::shared_ptr<VertexBuffer> Generate::Cuboid(float width, float height, float depth, glm::vec3 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(8);

	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	//front
	vertBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	//right
	vertBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	//back
	vertBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	//left
	vertBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	//top
	vertBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	//bottom
	vertBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	#pragma endregion

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 3, 3);
	vertBuf->AddVertexAttribute("texCoord", 2, 2, 6);

    return vertBuf;
}*/

std::shared_ptr<Mesh> Generate::CuboidWithNormals(float width, float height, float depth, float u, float v, glm::vec4 color) {

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(12);
	VertexBuffer& vBuf = mesh->GetVertexBuffer();
	IndexBuffer& iBuf = mesh->GetIndexBuffer();

	vBuf.AddVertexAttribute("position", 0, 3, 0);
	vBuf.AddVertexAttribute("vertexColor", 1, 4, 3);
	vBuf.AddVertexAttribute("vertexNormal", 2, 3, 7);
	vBuf.AddVertexAttribute("texCoord", 3, 2, 10);
	unsigned int indexOffset = 0;
	
	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;


	//front
	vBuf.AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, v);
	vBuf.AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, 0.0f);
	vBuf.AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, v);
	vBuf.AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, 0.0f);
	iBuf.AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	iBuf.AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//back
	vBuf.AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, u, v);
	vBuf.AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, u, 0.0f);
	vBuf.AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, v);
	vBuf.AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, 0.0f);
	iBuf.AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	iBuf.AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//left
	vBuf.AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, u);
	vBuf.AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, v, u);
	vBuf.AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, 0.0f);
	vBuf.AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, v, 0.0f);
	iBuf.AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	iBuf.AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//right
	vBuf.AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, u);
	vBuf.AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, v, u);
	vBuf.AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, 0.0f);
	vBuf.AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, v, 0.0f);
	iBuf.AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	iBuf.AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;
	
	//top
	vBuf.AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, 0, v);
	vBuf.AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0, 0.0f);
	vBuf.AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, u, v);
	vBuf.AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, u, 0.0f);
	iBuf.AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	iBuf.AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//bottom
	vBuf.AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_UP, u, v);
	vBuf.AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, u, 0.0f);
	vBuf.AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, v);
	vBuf.AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, 0.0f);
	iBuf.AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	iBuf.AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	return mesh;
}

std::shared_ptr<Mesh> Generate::PlaneXZWithNormals(float width, float depth, float u, float v, glm::vec4 color) {

	auto mesh = std::make_shared<Mesh>(12);
	VertexBuffer& vBuf = mesh->GetVertexBuffer();
	IndexBuffer& iBuf = mesh->GetIndexBuffer();

	vBuf.AddVertexAttribute("position", 0, 3, 0);
	vBuf.AddVertexAttribute("vertexColor", 1, 4, 3);
	vBuf.AddVertexAttribute("vertexNormal", 2, 3, 7);
	vBuf.AddVertexAttribute("texCoord", 3, 2, 10);

	float hw = width / 2;
	float hd = depth / 2;

	vBuf.AddVertexData(12, -hw, 0.0f, hd, color.r, color.g, color.b, color.a, NORM_UP, u, v);
	vBuf.AddVertexData(12, -hw, 0.0f, -hd, color.r, color.g, color.b, color.a, NORM_UP, u, 0.0f);
	vBuf.AddVertexData(12, hw, 0.0f, hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, v);
	vBuf.AddVertexData(12, hw, 0.0f, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, 0.0f);
	iBuf.AddIndexData(3, 0, 2, 3);
	iBuf.AddIndexData(3, 0, 3, 1);
	
	return mesh;
}

std::shared_ptr<Mesh> Generate::PlaneXYWithNormals(float width, float height, float u, float v, glm::vec4 color) {

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(12);
	VertexBuffer& vBuf = mesh->GetVertexBuffer();
	IndexBuffer& iBuf = mesh->GetIndexBuffer();

	vBuf.AddVertexAttribute("position", 0, 3, 0);
	vBuf.AddVertexAttribute("vertexColor", 1, 4, 3);
	vBuf.AddVertexAttribute("vertexNormal", 2, 3, 7);
	vBuf.AddVertexAttribute("texCoord", 3, 2, 10);
	
	float hw = width / 2;
	float hh = height / 2;

	vBuf.AddVertexData(12, hw, hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, v);
	vBuf.AddVertexData(12, hw, -hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, 0.0f);
	vBuf.AddVertexData(12, -hw, hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, v);
	vBuf.AddVertexData(12, -hw, -hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, 0.0f);
	iBuf.AddIndexData(3, 0, 2, 3);
	iBuf.AddIndexData(3, 0, 3, 1);

	return mesh;
}

std::shared_ptr<Mesh> Generate::CylinderWithNormals(float radius, float height, unsigned int numSegments, const glm::vec4& color) {

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(12);
	VertexBuffer& vBuf = mesh->GetVertexBuffer();
	IndexBuffer& iBuf = mesh->GetIndexBuffer();

	vBuf.AddVertexAttribute("position", 0, 3, 0);
	vBuf.AddVertexAttribute("vertexColor", 1, 4, 3);
	vBuf.AddVertexAttribute("vertexNormal", 2, 3, 7);
	vBuf.AddVertexAttribute("texCoord", 3, 2, 10);

	float hh = height / 2;
	double tau = 2 * M_PI;
	double delta = tau / numSegments;
	for (double theta = 0.0f; theta < tau; theta += delta) {
		double c = cos(theta);
		double s = sin(theta);
		double rc = radius * c;
		double rs = radius * s;

		//because we are using indices and those don't play well with normals, we need two copies of each rim
		//upper rim, up facing
		vBuf.AddVertexData(12, rc, hh, rs, color.r, color.g, color.b, color.a, NORM_UP, c + 0.5f, s + 0.5f);
		//lower rim, down facing
		vBuf.AddVertexData(12, rc, -hh, rs, color.r, color.g, color.b, color.a, NORM_DOWN, c + 0.5f, s + 0.5f);
		//upper rim, outward facing
		vBuf.AddVertexData(12, rc, hh, rs, color.r, color.g, color.b, color.a, c, 0.0f, s, theta / tau, 1.0f);
		//lower rim, outward facing
		vBuf.AddVertexData(12, rc, -hh, rs, color.r, color.g, color.b, color.a, c, 0.0f, s, theta / tau, 0.0f);
	}

	for (size_t i = 0; i < numSegments - 1; i++) {
		iBuf.AddIndexData(3, 0, 4 * (i + 1), 4 * i); //part of upper rim
		iBuf.AddIndexData(3, 0 + 1, 4 * i + 1, 4 * (i + 1) + 1); //part of lower rim
		iBuf.AddIndexData(3, 4 * i + 2, 4 * (i + 1) + 2, 4 * i + 3); //upper triangle of side face
		iBuf.AddIndexData(3, 4 * (i + 1) + 2, 4 * (i + 1) + 3, 4 * i + 3); //lower triangle of side face
	}

	//close up the ring with two final triangles
	iBuf.AddIndexData(3, 4 * (numSegments - 1) + 2, 2, 4 * (numSegments - 1) + 3);
	iBuf.AddIndexData(3, 2, 3, 4 * (numSegments - 1) + 3);

	return mesh;
}

/*
std::shared_ptr<IndexBuffer> Generate::LineCylinderIndices(unsigned int numSegments, bool isClosed) {
	
	auto idxBuf = std::make_shared<IndexBuffer>();
		
	std::size_t i = 0;
	for (i = 0; i < (numSegments - 1) * 2; i += 2) {
		idxBuf->AddIndexData(2, i, i + 2); //top circle
		idxBuf->AddIndexData(2, i + 1, i + 3); //bottom half
		idxBuf->AddIndexData(2, i, i + 1); //stitch them together
	}
	if (isClosed) {
		idxBuf->AddIndexData(2, i, 0); //top
		idxBuf->AddIndexData(2, i + 1, 1); //bottom
		idxBuf->AddIndexData(2, i, i + 1); //stitch
	}

	return idxBuf;
}

std::shared_ptr<VertexBuffer> Generate::LineSphereVertices(float radius, unsigned int numSegments, const glm::vec3& color) {
	throw "not impl";
}

std::shared_ptr<IndexBuffer> Generate::LineSphereIndices(unsigned int numSegments) {
	throw "not impl";
}

std::shared_ptr<VertexBuffer> Generate::LineQuadSphereVertices(float radius, unsigned int resolution, const glm::vec3& color) {
	throw "not impl";
}

std::shared_ptr<IndexBuffer> Generate::LineQuadSphereIndices(unsigned int resolution) {
	throw "not impl";
}*/
