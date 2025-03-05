#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include <variant>

namespace Cicada {

class Material : public ManagedObject<Material> {
friend class ManagedObject<Material>;
private:
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Texture2D> m_texture;
public:
	Material(std::string_view, const std::shared_ptr<Shader>&, const std::shared_ptr<Texture2D>&);

public:
	~Material() = default;

	inline const std::shared_ptr<Shader>& GetShader() { return m_shader; }
	inline void SetShader(const std::shared_ptr<Shader>& shader) { m_shader = shader; }

	inline const std::shared_ptr<Texture2D>& GetTexture() { return m_texture; }
	inline void SetTexture(const std::shared_ptr<Texture2D>& texture) { m_texture = texture; }
	
	void Bind() const;

	void SetInt(std::string_view uniformName, int value) const;
	void SetUInt(std::string_view uniformName, unsigned int value) const;
	void SetFloat(std::string_view uniformName, float value) const;
	void SetVec3(std::string_view uniformName, const glm::vec3& value) const;
	void SetMat4(std::string_view uniformName, const glm::mat4& value) const;
	//TODO: these are the core most necessary ones, but add more later

	//TODO: add any necessary manipulator functions for the texture too
};

}