#pragma once
#include "Behavior.h"
#include "BoundingBox.h"
#include "GraphicsStructures.h"
#include "Material.h"
#include "Mesh.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class GameObject {
protected:
	glm::mat4 referenceFrame;
	
	std::unordered_set<std::shared_ptr<GameObject>> children;
	GameObject* parent;
	std::shared_ptr<BoundingBox> boundingBox;

	std::shared_ptr<Material> m_material;
	std::shared_ptr<Texture2D> texture;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Material_OLD> material;

	std::shared_ptr<Mesh> m_mesh;

public:
	inline GameObject() :
		referenceFrame(1.0f), m_mesh(nullptr),
		parent(nullptr), material(nullptr), texture(nullptr) {
	}
	inline virtual ~GameObject() = default;

	//gets the reference frame of this object in global world space
	inline const glm::mat4 GetGlobalReferenceFrame() const {
		//do this by working our way up the object hierarchy until we hit the root object with no parent
		if (parent != nullptr)
			return referenceFrame * parent->GetGlobalReferenceFrame();
		return referenceFrame;
	}

	//returns the reference frame of this object, regardless of whether or not it is parented
	inline const glm::mat4& GetLocalReferenceFrame() const { return referenceFrame; }
	
	//sets the reference frame of this object, not taking into account the parent
	inline void SetLocalReferenceFrame(const glm::mat4& referenceFrame) { this->referenceFrame = referenceFrame; }
	
	inline const std::shared_ptr<Mesh>& GetMesh() const { return m_mesh; }
	inline void SetMesh(const std::shared_ptr<Mesh>& mesh) { this->m_mesh = mesh; }

	//returns a const reference to the child container
	inline const std::unordered_set<std::shared_ptr<GameObject>>& GetChildren() const { return children; }

	//inserts a new child under this object, returns true if success, false if already present
	bool AddChild(const std::shared_ptr<GameObject>& child);

	inline const std::shared_ptr<Texture2D>& GetTexture() const { return texture; }
	inline void SetTexture(const std::shared_ptr<Texture2D>& texture) { this->texture = texture; }
	inline bool HasTexture() const { return texture != nullptr; }

	inline void SelectTexture(void) const {
		if (texture != nullptr)
			texture->SelectForRendering();
	}

	inline const std::shared_ptr<Shader>& GetShader() const { return shader; }
	inline void SetShader(const std::shared_ptr<Shader>& shader) { this->shader = shader; }

	inline const std::shared_ptr<Material_OLD>& GetMaterial_OLD() const { return material; }
	inline const std::shared_ptr<Material>& GetMaterial() const { return m_material; }
	inline void SetMaterial(const std::shared_ptr<Material_OLD>& material) { this->material = material; }
	inline void SetMaterial(const std::shared_ptr<Material>& material) { m_material = material; }

	inline bool HasBoundingBox() const { return boundingBox != nullptr; }
	inline const std::shared_ptr<BoundingBox>& GetBoundingBox() const { return boundingBox; }
	inline void SetBoundingBox(const std::shared_ptr<BoundingBox>& boundingBox) { this->boundingBox = boundingBox; }
	bool IsIntersectingRay(const Ray& ray) const;

	const glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& position);

	void ResetOrientation();
	void RotateLocalX(float degrees);
	void RotateLocalY(float degrees);
	void RotateLocalZ(float degrees);
	void RotateLocal(float xDeg, float yDeg, float zDeg);
	void RotateToFace(const glm::vec3& target);

	void StaticAllocate() const;
	void Update(double elapsedSeconds);
};
