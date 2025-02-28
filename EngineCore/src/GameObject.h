#pragma once
#include "BoundingBox.h"
#include "GraphicsStructures.h"
#include "core/Material.h"
#include "core/Mesh.h"
#include "Ray.h"

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace Cicada {

class GameObject {
protected:
	glm::mat4 m_localTransform = glm::mat4(1.0f);
	
	GameObject* m_parent = nullptr;
	std::unordered_set<std::shared_ptr<GameObject>> m_children;
	
	std::shared_ptr<BoundingBox> m_boundingBox = nullptr;
	std::shared_ptr<Mesh> m_mesh = nullptr;
	std::shared_ptr<Material> m_material = nullptr;
	std::shared_ptr<Material_OLD> m_material_OLD = nullptr;

public:
	inline GameObject(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) :
		m_mesh(mesh),
		m_material(material),
		m_parent(nullptr) {
	}

	//gets the transform of this object in global world space
	inline const glm::mat4 GetGlobalTransform() const {
		//do this by working our way up the object hierarchy until we hit the root object with no parent
		if (m_parent != nullptr)
			return m_localTransform * m_parent->GetGlobalTransform();
		return m_localTransform;
	}

	//returns the transform of this object, regardless of whether or not it is parented
	inline const glm::mat4& GetLocalTransform() const { return m_localTransform; }
	
	//sets the transform of just this object, not taking into account the parent
	inline void SetLocalTransform(const glm::mat4& transform) { this->m_localTransform = transform; }
	
	inline const std::shared_ptr<Mesh>& GetMesh() const { return m_mesh; }
	inline void SetMesh(const std::shared_ptr<Mesh>& mesh) { this->m_mesh = mesh; }

	//returns a const reference to the child container
	inline const std::unordered_set<std::shared_ptr<GameObject>>& GetChildren() const { return m_children; }

	//inserts a new child under this object, returns true if success, false if already present
	bool AddChild(const std::shared_ptr<GameObject>& child);
	
	//TODO: implemenet SetParent

	inline const std::shared_ptr<Material_OLD>& GetMaterial_OLD() const { return m_material_OLD; }
	inline const std::shared_ptr<Material>& GetMaterial() const { return m_material; }
	inline void SetMaterial_OLD(const std::shared_ptr<Material_OLD>& material) { m_material_OLD = material; }
	inline void SetMaterial(const std::shared_ptr<Material>& material) { m_material = material; }

	inline bool HasBoundingBox() const { return m_boundingBox != nullptr; }
	inline const std::shared_ptr<BoundingBox>& GetBoundingBox() const { return m_boundingBox; }
	inline void SetBoundingBox(const std::shared_ptr<BoundingBox>& boundingBox) { this->m_boundingBox = boundingBox; }
	bool IsIntersectingRay(const Ray& ray) const;

	const glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);

	void ResetOrientation();
	void RotateLocalX(float degrees);
	void RotateLocalY(float degrees);
	void RotateLocalZ(float degrees);
	void RotateLocal(float xDeg, float yDeg, float zDeg);
	void RotateToFace(const glm::vec3& target);

	void UploadResources() const;
	void Update(double elapsedSeconds);
};

}
