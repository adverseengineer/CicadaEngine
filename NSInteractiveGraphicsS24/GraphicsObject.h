#pragma once
#include "Animation.h"
#include "GraphicsStructures.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_set>

//forward decl so that we can resolve a cyclical dependency
class Animation;

class GraphicsObject {
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> vertBuf;
	std::unordered_set<std::shared_ptr<GraphicsObject>> children;
	GraphicsObject* parent;
	std::shared_ptr<Animation> animation;
	std::shared_ptr<Material> material;

public:
	inline GraphicsObject() : referenceFrame(1.0f), parent(nullptr), material(nullptr) {}
	inline virtual ~GraphicsObject() = default;

	//gets the reference frame of this object in global world space
	inline const glm::mat4& GetGlobalReferenceFrame() const {
		//do this by working our way up the object hierarchy until we hit the root object with no parent
		if (parent != nullptr)
			return referenceFrame * parent->GetGlobalReferenceFrame();
		return referenceFrame;
	}

	//returns the reference frame of this object, regardless of whether or not it is parented
	inline const auto& GetLocalReferenceFrame() const { return referenceFrame; }
	
	//sets the reference frame of this object, not taking into account the parent
	inline void SetLocalReferenceFrame(const glm::mat4& referenceFrame) { this->referenceFrame = referenceFrame; }
	
	inline const auto& GetVertexBuffer() const { return vertBuf; }
	inline void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertBuf) { this->vertBuf = vertBuf; }

	//returns a const reference to the child container
	inline const auto& GetChildren() const { return children; }

	//inserts a new child under this object, returns true if success, false if already present
	bool AddChild(const std::shared_ptr<GraphicsObject>& child);

	inline const auto& GetAnimation() const { return animation; }
	inline void SetAnimation(const std::shared_ptr<Animation>& animation) { this->animation = animation; }

	inline const auto& GetMaterial() const { return material; }
	inline void SetMaterial(const std::shared_ptr<Material>& material) { this->material = material; }

	const auto& GetPosition() const;
	void SetPosition(const glm::vec3& position);

	void ResetOrientation();
	void RotateLocalX(float degrees);
	void RotateLocalY(float degrees);
	void RotateLocalZ(float degrees);
	void RotateLocal(float xDeg, float yDeg, float zDeg);

	void StaticAllocateVertexBuffer() const;
	void Update(double elapsedSeconds);
};
