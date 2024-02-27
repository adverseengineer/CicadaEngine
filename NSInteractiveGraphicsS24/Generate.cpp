#include "Generate.h"

std::shared_ptr<VertexBuffer> Generate::Cuboid(float width, float height, float depth, glm::vec3 color, glm::vec2 tex) {

	auto vBufPtr = std::make_shared<VertexBuffer>(8);

	//3 vertex per triangle, 2 triangles per face, 6 faces
	//3*2*6 = 36 vertices

	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	//front
	vBufPtr->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	vBufPtr->AddVertexData(8, -hw, -hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	vBufPtr->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	vBufPtr->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	vBufPtr->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	vBufPtr->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);
	//right
	vBufPtr->AddVertexData(8, hw, hh, hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, hw, -hh, hd, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, hh, hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//back
	vBufPtr->AddVertexData(8, hw, hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//left
	vBufPtr->AddVertexData(8, -hw, hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//top
	vBufPtr->AddVertexData(8, -hw, hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, -hw, hh, hd, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, hw, hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//bottom
	vBufPtr->AddVertexData(8, hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, hw, -hh, hd, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, -hh, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	#pragma endregion

	//add vertex attributes to the vertex buffer
	vBufPtr->AddVertexAttribute("position", 0, 3, 0);
	vBufPtr->AddVertexAttribute("vertexColor", 1, 3, 3);
	vBufPtr->AddVertexAttribute("texCoord", 2, 2, 6);

    return vBufPtr;
}

std::shared_ptr<VertexBuffer> Generate::Plane(float width, float depth, glm::vec3 color, glm::vec2 tex) {
	
	auto vBufPtr = std::make_shared<VertexBuffer>(8);

	float hw = width / 2;
	float hd = depth / 2;

	vBufPtr->AddVertexData(8, -hw, 0, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, -hw, 0, hd, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, 0, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, -hw, 0, -hd, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	vBufPtr->AddVertexData(8, hw, 0, hd, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vBufPtr->AddVertexData(8, hw, 0, -hd, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	//add vertex attributes to the vertex buffer
	vBufPtr->AddVertexAttribute("position", 0, 3, 0);
	vBufPtr->AddVertexAttribute("vertexColor", 1, 3, 3);
	vBufPtr->AddVertexAttribute("texCoord", 2, 2, 6);

	return vBufPtr;
}