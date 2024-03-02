#pragma once
#include "Animation.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <memory>

class Animation;

class GraphicsObject {
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> vBuf;
	GraphicsObject* parent;
	std::vector<std::shared_ptr<GraphicsObject>> children;
	std::shared_ptr<Animation> animation;

public:
	GraphicsObject(void);
	virtual ~GraphicsObject(void) = default;

	glm::mat4 GetReferenceFrame(void) const;
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

	void Update(double elapsedSeconds);
	inline void SetAnimation(const std::shared_ptr<Animation>& animation) { this->animation = animation; }
	inline glm::mat4& GetLocalReferenceFrame(void) { return referenceFrame; }
};
