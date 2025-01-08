#pragma once

#include "Mesh.h"
#include <glm.hpp>

class Generate {

public:
	static std::shared_ptr<Mesh> CuboidWithNormals(float width, float height, float depth, float u, float v, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<Mesh> PlaneXZWithNormals(float width, float depth, float u, float v, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<Mesh> PlaneXYWithNormals(float width, float height, float u, float v, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });	
	static std::shared_ptr<Mesh> CylinderWithNormals(float radius, float height, unsigned int numSegments = 12, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<Mesh> PolarSphereWithNormals(float radius, unsigned int latitudes, unsigned int longitudes, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
};
