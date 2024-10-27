#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include <variant>

//TODO: add more types as needed
//using MaterialParamValueType = std::variant<
//	int, unsigned int, float, double,
//	glm::vec3, glm::vec4, glm::mat4
//>;

struct MaterialParam {
	UniformInfo m_info;
	//MaterialParamValueType m_value;
	float m_value; //TODO: change back to variant once i know wtf im doing
};

class Material {

private:
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Texture2D> m_texture;
	std::unordered_map<std::string, MaterialParam> m_params;

public:
	inline Material(const std::shared_ptr<Shader> shader, const std::shared_ptr<Texture2D>& texture) : m_shader(shader), m_texture(texture) {
	}
	~Material() = default;

	const std::shared_ptr<Shader>& GetShader() { return m_shader; }
	void SetShader(const std::shared_ptr<Shader>& shader) { m_shader = shader; }

	const std::shared_ptr<Texture2D>& GetTexture() { return m_texture; }
	void SetTexture(const std::shared_ptr<Texture2D>& texture) { m_texture = texture; }

	void GetUniforms();
	void SendParams() const;
	void DBG_ShowInfo() const;
};
