#include "Generate.h"

std::shared_ptr<VertexBuffer> Generate::Cuboid(float width, float height, float depth, glm::vec3 color, glm::vec2 tex) {

	auto vBuf = std::make_shared<VertexBuffer>(8);

	//3 vertex per triangle, 2 triangles per face, 6 faces
	//3*2*6 = 36 vertices

	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	//front
	vBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	//right
	vBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	//back
	vBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	//left
	vBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	//top
	vBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	//bottom
	vBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, -hh, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, -hh, -hd, color.r, color.g, color.b, tex.s, tex.t);
	#pragma endregion

	//add vertex attributes to the vertex buffer
	vBuf->AddVertexAttribute("position", 0, 3, 0);
	vBuf->AddVertexAttribute("vertexColor", 1, 3, 3);
	vBuf->AddVertexAttribute("texCoord", 2, 2, 6);

    return vBuf;
}

std::shared_ptr<VertexBuffer> Generate::CuboidWithNormals(float width, float height, float depth, glm::vec4 color, glm::vec2 tex) {

	auto vBuf = std::make_shared<VertexBuffer>(12);
	//vertPos (3), color (4), norm (3), uv (2)

	//3 vertex per triangle, 2 triangles per face, 6 faces
	//3*2*6 = 36 vertices


	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	#define up 0.0f, 1.0f, 0.0f
	#define down 0.0f, -1.0f, 0.0f
	#define left -1.0f, 0.0f, 0.0f
	#define right 1.0f, 0.0f, 0.0f
	#define forward 0.0f, 0.0f, 1.0f
	#define backward 0.0f, 0.0f, -1.0f

	//front
	vBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, backward, 0.0f, tex.t);
	vBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, backward, 0.0f, 0.0f);
	vBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, backward, tex.s, 0.0f);
	vBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, backward, 0.0f, tex.t);
	vBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, backward, tex.s, 0.0f);
	vBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, backward, tex.s, tex.t);
	//right
	vBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, right, 0.0f, tex.t);
	vBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, right, 0.0f, 0.0f);
	vBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, right, tex.s, 0.0f);
	vBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, right, 0.0f, tex.t);
	vBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, right, tex.s, 0.0f);
	vBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, right, tex.s, tex.t);
	//back
	vBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, forward, 0.0f, tex.t);
	vBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, forward, 0.0f, 0.0f);
	vBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, forward, tex.s, 0.0f);
	vBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, forward, 0.0f, tex.t);
	vBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, forward, tex.s, 0.0f);
	vBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, forward, tex.s, tex.t);
	//left
	vBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, left, 0.0f, tex.t);
	vBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, left, 0.0f, 0.0f);
	vBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, left, tex.s, 0.0f);
	vBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, left, 0.0f, tex.t);
	vBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, left, tex.s, 0.0f);
	vBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, left, tex.s, tex.t);
	//top
	vBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vBuf->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, up, 0.0f, 0.0f);
	vBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vBuf->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vBuf->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vBuf->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, up, tex.s, tex.t);
	//bottom
	vBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, down, 0.0f, tex.t);
	vBuf->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, down, 0.0f, 0.0f);
	vBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, down, tex.s, 0.0f);
	vBuf->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, down, 0.0f, tex.t);
	vBuf->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, down, tex.s, 0.0f);
	vBuf->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, down, tex.s, tex.t);
	#pragma endregion

	//add vertex attributes to the vertex buffer
	vBuf->AddVertexAttribute("position", 0, 3, 0);
	vBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vBuf->AddVertexAttribute("normal", 2, 3, 7);
	vBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vBuf;
}

std::shared_ptr<VertexBuffer> Generate::Plane(float width, float depth, glm::vec3 color, glm::vec2 tex) {
	
	auto vBuf = std::make_shared<VertexBuffer>(12);

	float hw = width / 2;
	float hd = depth / 2;

	vBuf->AddVertexData(8, -hw, 0, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, 0, hd, color.r, color.g, color.b, 0.0f, 0.0f);
	vBuf->AddVertexData(8, hw, 0, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, 0, -hd, color.r, color.g, color.b, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, 0, hd, color.r, color.g, color.b, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, 0, -hd, color.r, color.g, color.b, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vBuf->AddVertexAttribute("position", 0, 3, 0);
	vBuf->AddVertexAttribute("vertexColor", 1, 3, 3);
	vBuf->AddVertexAttribute("texCoord", 2, 2, 6);

	return vBuf;
}

std::shared_ptr<VertexBuffer> Generate::PlaneWithNormals(float width, float depth, glm::vec4 color, glm::vec2 tex) {

	auto vBuf = std::make_shared<VertexBuffer>(12);

	float hw = width / 2;
	float hd = depth / 2;

	vBuf->AddVertexData(8, -hw, 0, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vBuf->AddVertexData(8, -hw, 0, hd, color.r, color.g, color.b, color.a, up, 0.0f, 0.0f);
	vBuf->AddVertexData(8, hw, 0, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vBuf->AddVertexData(8, -hw, 0, -hd, color.r, color.g, color.b, color.a, up, 0.0f, tex.t);
	vBuf->AddVertexData(8, hw, 0, hd, color.r, color.g, color.b, color.a, up, tex.s, 0.0f);
	vBuf->AddVertexData(8, hw, 0, -hd, color.r, color.g, color.b, color.a, up, tex.s, tex.t);

	//add vertex attributes to the vertex buffer
	vBuf->AddVertexAttribute("position", 0, 3, 0);
	vBuf->AddVertexAttribute("vertexColor", 1, 4, 3);
	vBuf->AddVertexAttribute("normal", 2, 3, 7);
	vBuf->AddVertexAttribute("texCoord", 3, 2, 10);

	return vBuf;
}
