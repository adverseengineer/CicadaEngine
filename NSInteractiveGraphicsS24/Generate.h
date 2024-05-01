#pragma once
#include "IndexBuffer.h"
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

	static std::shared_ptr<VertexBuffer> LineCircleVertices(float radius, unsigned int numSegments = 12, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<IndexBuffer> LineCircleIndices(unsigned int numSegments, bool isClosed = true);

	static std::shared_ptr<VertexBuffer> LineCylinderVertices(float radius, float height, unsigned int numSegments = 12, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<IndexBuffer> LineCylinderIndices(unsigned int numSegments = 12, bool isClosed = true);

	static std::shared_ptr<VertexBuffer> LineSphereVertices(float radius, unsigned int numSegments = 12, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<IndexBuffer> LineSphereIndices(unsigned int numSegments = 12);
	
	static std::shared_ptr<VertexBuffer> PolarSphere(float radius, unsigned int slices, unsigned int stacks, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	static std::shared_ptr<VertexBuffer> QuadSphere(float radius, unsigned int resolution = 8, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
};
