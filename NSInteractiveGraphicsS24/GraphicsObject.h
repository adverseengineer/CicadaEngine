#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "VertexBuffer.h"

class GraphicsObject {
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> vBuf;
	GraphicsObject* parent;
	std::vector<std::shared_ptr<GraphicsObject>> children;

public:
	GraphicsObject();
	virtual ~GraphicsObject() = default;

	const glm::mat4 GetReferenceFrame() const;
	void CreateVertexBuffer(unsigned int numberOfElementsPerVertex);
	void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuf);
	inline const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return vBuf; }
	inline void SetReferenceFrame(const glm::mat4& referenceFrame) { this->referenceFrame = referenceFrame; }
	void StaticAllocateVertexBuffer(void);

	void AddChild(const std::shared_ptr<GraphicsObject>& child);
	inline const std::vector<std::shared_ptr<GraphicsObject>>& GetChildren() const { return children; }

	void SetPosition(const glm::vec3& position);
	void ResetOrientation();
	void RotateLocalX(float degrees);
	void RotateLocalY(float degrees);
	void RotateLocalZ(float degrees);
	void RotateLocal(float xDeg, float yDeg, float zDeg);
};
