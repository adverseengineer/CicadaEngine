#pragma once
#include "AnimationBehavior.h"
#include "BoundingBox.h"
#include "GraphicsStructures.h"
#include "IndexBuffer.h"
#include "Ray.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>

//forward decl so that we can resolve a cyclical dependency
class Animation;

class GraphicsObject {
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> vertBuf;
	std::shared_ptr<IndexBuffer> idxBuf;
	std::unordered_set<std::shared_ptr<GraphicsObject>> children;
	GraphicsObject* parent;
	std::unordered_map<std::string, std::shared_ptr<Behavior>> behaviorMap;
	std::shared_ptr<Material> material;
	std::shared_ptr<BoundingBox> boundingBox;

public:
	inline GraphicsObject() :
		referenceFrame(1.0f), vertBuf(nullptr), idxBuf(nullptr),
		parent(nullptr), material(nullptr) {
	}
	inline virtual ~GraphicsObject() = default;

	//gets the reference frame of this object in global world space
	inline const glm::mat4& GetGlobalReferenceFrame() const {
		//do this by working our way up the object hierarchy until we hit the root object with no parent
		if (parent != nullptr)
			return referenceFrame * parent->GetGlobalReferenceFrame();
		return referenceFrame;
	}

	//returns the reference frame of this object, regardless of whether or not it is parented
	inline const glm::mat4& GetLocalReferenceFrame() const { return referenceFrame; }
	
	//sets the reference frame of this object, not taking into account the parent
	inline void SetLocalReferenceFrame(const glm::mat4& referenceFrame) { this->referenceFrame = referenceFrame; }
	
	inline const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return vertBuf; }
	inline void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertBuf) { this->vertBuf = vertBuf; }

	inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return idxBuf; }
	inline void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& idxBuf) { this->idxBuf = idxBuf; }
	inline bool IsIndexed() const { return idxBuf != nullptr; }
	inline void CreateIndexBuffer() const {
		throw "not impl";
	}

	//returns a const reference to the child container
	inline const std::unordered_set<std::shared_ptr<GraphicsObject>>& GetChildren() const { return children; }

	//inserts a new child under this object, returns true if success, false if already present
	bool AddChild(const std::shared_ptr<GraphicsObject>& child);

	inline bool AddBehavior(const std::string& behaviorName, const std::shared_ptr<Behavior>& behavior) { 
		return behaviorMap.insert_or_assign(behaviorName, behavior).second;
	}
	inline void SetBehaviorDefaults() {
		for (auto& [name, behavior] : behaviorMap) {
			behavior->StoreDefaults();
		}
	}
	inline void SetBehaviorParameters(const std::string& name, BehaviorParams& params) {
		auto& behavior = behaviorMap.at(name);
		behavior->SetParameter(params);
	}

	inline const std::shared_ptr<Material>& GetMaterial() const { return material; }
	inline void SetMaterial(const std::shared_ptr<Material>& material) { this->material = material; }

	inline bool HasBoundingBox() const { return boundingBox != nullptr; }
	inline const std::shared_ptr<BoundingBox>& GetBoundingBox() const { return boundingBox; }
	inline void SetBoundingBox(const std::shared_ptr<BoundingBox>& boundingBox) { this->boundingBox = boundingBox; }
	bool IsIntersectingRay(const Ray& ray) const;

	const glm::vec3& GetPosition() const;
	void SetPosition(const glm::vec3& position);

	void ResetOrientation();
	void RotateLocalX(float degrees);
	void RotateLocalY(float degrees);
	void RotateLocalZ(float degrees);
	void RotateLocal(float xDeg, float yDeg, float zDeg);
	void RotateToFace(const glm::vec3& target);

	void StaticAllocateBuffers() const;
	void Update(double elapsedSeconds);
};
