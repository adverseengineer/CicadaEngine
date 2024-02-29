#pragma once

#include "VertexBuffer.h"
#include <glm.hpp>

class Generate {

public:
	static std::shared_ptr<VertexBuffer> Cuboid(float width, float height, float depth, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> Plane(float width, float depth, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, glm::vec2 tex = { 1.0f, 1.0f });
};

