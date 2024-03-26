#pragma once
#include "Shader.h"
#include <string>
#include <unordered_map>

class ShaderManager {
private:
	static std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;

public:
	static const std::shared_ptr<Shader>& GetShader(const std::string& key);
	static bool AddShader(const std::string& key, const std::shared_ptr<Shader>& obj);
	static bool SetShader(const std::string& key, const std::shared_ptr<Shader>& obj);
};
