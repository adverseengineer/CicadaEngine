#include "Generate.h"

#define up 0.0f, 1.0f, 0.0f
#define down 0.0f, -1.0f, 0.0f
#define left -1.0f, 0.0f, 0.0f
#define right 1.0f, 0.0f, 0.0f
#define forward 0.0f, 0.0f, -1.0f
#define backward 0.0f, 0.0f, 1.0f

std::shared_ptr<VertexBuffer> Generate::Cuboid(float width, float height, float depth, glm::vec3 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(8);

	//3 vertex per triangle, 2 triangles per face, 6 faces
	//3*2*6 = 36 vertices

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
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, backward, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, backward, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, backward, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, backward, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, backward, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, backward, tex.s, tex.t);
	//right
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, right, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, right, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, right, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, right, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, right, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, right, tex.s, tex.t);
	//back
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, forward, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, forward, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, forward, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, forward, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, forward, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, forward, tex.s, tex.t);
	//left
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, left, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, left, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, left, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, left, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, left, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, left, tex.s, tex.t);
	//top
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, up, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, up, tex.s, tex.t);
	//bottom
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, down, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, down, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, down, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, down, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, down, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, down, tex.s, tex.t);
	#pragma endregion

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vertBuf->AddVertexAttribute("vertexNormal", 2, 3, 7);
	vertBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vertBuf;
}

std::shared_ptr<VertexBuffer> Generate::Plane(float width, float depth, glm::vec3 color, glm::vec2 tex) {
	
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

std::shared_ptr<VertexBuffer> Generate::PlaneWithNormals(float width, float depth, glm::vec4 color, glm::vec2 tex) {

	auto vertBuf = std::make_shared<VertexBuffer>(12);

	float hw = width / 2;
	float hd = depth / 2;

	vertBuf->AddVertexData(12, -hw, 0, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vertBuf->AddVertexData(12, -hw, 0, hd, color.r, color.g, color.b, color.a, up, 0.0f, 0.0f);
	vertBuf->AddVertexData(12, hw, 0, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vertBuf->AddVertexData(12, -hw, 0, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vertBuf->AddVertexData(12, hw, 0, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vertBuf->AddVertexData(12, hw, 0, -hd, color.r, color.g, color.b, color.a, up, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vertBuf->AddVertexAttribute("position", 0, 3, 0);
	vertBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vertBuf->AddVertexAttribute("vertexNormal", 2, 3, 7);
	vertBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vertBuf;
}
