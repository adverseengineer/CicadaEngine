#pragma once
#include "VertexBuffer.h"
#include <glm.hpp>

class Generate {

public:
	static std::shared_ptr<VertexBuffer> Cuboid(float width, float height, float depth, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> CuboidWithNormals(float width, float height, float depth, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> PlaneXZ(float width, float depth, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> PlaneXY(float width, float height, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> PlaneXZWithNormals(float width, float depth, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> PlaneXYWithNormals(float width, float height, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });	
};
