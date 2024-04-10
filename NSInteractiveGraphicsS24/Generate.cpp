#include "Generate.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define NORM_UP 0.0f, 1.0f, 0.0f
#define NORM_DOWN 0.0f, -1.0f, 0.0f
#define NORM_LEFT -1.0f, 0.0f, 0.0f
#define NORM_RIGHT 1.0f, 0.0f, 0.0f
#define NORM_FORWARD 0.0f, 0.0f, -1.0f
#define NORM_BACKWARD 0.0f, 0.0f, 1.0f

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
}

std::shared_ptr<VertexBuffer> Generate::CuboidWithNormals(float width, float height, float depth, glm::vec4 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(12);
	//vertPos (3), color (4), norm (3), uv (2)

	//3 vertex per triangle, 2 triangles per face, 6 faces
	//3*2*6 = 36 vertices

	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	//front
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, tex.s, tex.t);
	//right
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, tex.s, tex.t);
	//back
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, tex.s, tex.t);
	//left
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, tex.s, tex.t);
	//top
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, tex.s, tex.t);
	//bottom
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_DOWN, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_DOWN, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_DOWN, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_DOWN, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_DOWN, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_DOWN, tex.s, tex.t);
	#pragma endregion

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vertBuf->AddVertexAttribute("vertexNormal", 2, 3, 7);
	vertBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vertBuf;
}

std::shared_ptr<VertexBuffer> Generate::PlaneXZ(float width, float depth, glm::vec3 color, glm::vec2 tex) {
	
	auto vertBuf = std::make_shared<VertexBuffer>(8);

	float hw = width / 2;
	float hd = depth / 2;

	vertBuf->AddVertexData(8, -hw, 0, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, 0, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, hw, 0, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, 0, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, 0, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, 0, -hd, color.r, color.g, color.b, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 3, 3);
	vertBuf->AddVertexAttribute("texCoord", 2, 2, 6);

	return vertBuf;
}

std::shared_ptr<VertexBuffer> Generate::PlaneXY(float width, float height, glm::vec3 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(8);

	float hw = width / 2;
	float hh = height / 2;

	vertBuf->AddVertexData(8, -hw, -hh, 0, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, -hw, hh, 0, color.r, color.g, color.b, 0.0f, 0.0f);
	vertBuf->AddVertexData(8, hw, hh, 0, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, -hw, -hh, 0, color.r, color.g, color.b, 0.0f, tex.t);
	vertBuf->AddVertexData(8, hw, hh, 0, color.r, color.g, color.b, tex.s, 0.0f);
	vertBuf->AddVertexData(8, hw, -hh, 0, color.r, color.g, color.b, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 3, 3);
	vertBuf->AddVertexAttribute("texCoord", 2, 2, 6);

	return vertBuf;
}

std::shared_ptr<VertexBuffer> Generate::PlaneXZWithNormals(float width, float depth, glm::vec4 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(12);

	float hw = width / 2;
	float hd = depth / 2;

	vertBuf->AddVertexData(12, -hw, 0, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, 0, hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, 0, hd, color.r, color.g, color.b, color.a, NORM_UP, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, 0, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, 0, hd, color.r, color.g, color.b, color.a, NORM_UP, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, 0, -hd, color.r, color.g, color.b, color.a, NORM_UP, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vertBuf->AddVertexAttribute("vertexNormal", 2, 3, 7);
	vertBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vertBuf;
}

std::shared_ptr<VertexBuffer> Generate::PlaneXYWithNormals(float width, float height, glm::vec4 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(12);

	float hw = width / 2;
	float hh = height / 2;

	//TODO: remove the need for this. the winding order is incorrect i think
	#define NORM_FORWARD_NEW 0.0f, 0.0f, 1.0f

	vertBuf->AddVertexData(12, -hw, hh, 0, color.r, color.g, color.b, color.a, NORM_FORWARD_NEW, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, 0, color.r, color.g, color.b, color.a, NORM_FORWARD_NEW, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, 0, color.r, color.g, color.b, color.a, NORM_FORWARD_NEW, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, 0, color.r, color.g, color.b, color.a, NORM_FORWARD_NEW, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, 0, color.r, color.g, color.b, color.a, NORM_FORWARD_NEW, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, 0, color.r, color.g, color.b, color.a, NORM_FORWARD_NEW, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vertBuf->AddVertexAttribute("vertexNormal", 2, 3, 7);
	vertBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vertBuf;
}

std::shared_ptr<VertexBuffer> Generate::LineCircleVertices(float radius, unsigned int numSegments, const glm::vec3& color) {

	auto vertBuf = std::make_shared<VertexBuffer>(6);
	
	float tau = 2 * M_PI;
	float radiansPerStep = tau / numSegments;
	for (float theta = 0.0f; theta < tau; theta += radiansPerStep)
		vertBuf->AddVertexData(6, radius * cos(theta), 0, radius * sin(theta), color.r, color.g, color.b);

	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 3, 3);

	return vertBuf;
}

std::shared_ptr<IndexBuffer> Generate::LineCircleIndices(unsigned int numSegments, bool isClosed) {

	auto idxBuf = std::make_shared<IndexBuffer>();

	std::size_t i;
	for (i = 0; i < numSegments - 1; i++)
		idxBuf->AddIndexData(2, i, i + 1);
	if(isClosed)
		idxBuf->AddIndexData(2, i, 0);

	return idxBuf;
}

std::shared_ptr<VertexBuffer> Generate::LineCylinderVertices(float radius, float height, unsigned int numSegments, const glm::vec3& color) {

	auto vertBuf = std::make_shared<VertexBuffer>(6);

	float hh = height / 2;
	float tau = 2 * M_PI;
	float radiansPerStep = tau / numSegments;
	for (float theta = 0.0f; theta < tau; theta += radiansPerStep) {
		float c = cos(theta);
		float s = sin(theta);
		vertBuf->AddVertexData(6, radius * c, hh, radius * s, color.r, color.g, color.b);
		vertBuf->AddVertexData(6, radius * c, -hh, radius * s, color.r, color.g, color.b);
	}

	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 3, 3);

	return vertBuf;
}

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
}
