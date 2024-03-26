#include "ShaderManager.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::shaderMap;

const std::shared_ptr<Shader>& ShaderManager::GetShader(const std::string& key) {
	//TODO: handle the possible out_of_range exception here
	return shaderMap.at(key);
}

bool ShaderManager::AddShader(const std::string& key, const std::shared_ptr<Shader>& obj) {
	const auto& result = shaderMap.insert({ key, obj });
	return result.second;
}

bool ShaderManager::SetShader(const std::string& key, const std::shared_ptr<Shader>& obj) {
	const auto& result = shaderMap.insert_or_assign(key, obj);
	return result.second;
}
