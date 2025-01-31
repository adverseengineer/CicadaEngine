#pragma once

#include "Logger.h"
#include "Shader.h"

#include <memory>

namespace Cicada {

class ShaderManager {

private:
	ShaderManager() = delete;
	static std::unordered_map<std::string, std::shared_ptr<Shader>> s_shaderMap;

public:
	inline static std::shared_ptr<Shader> GetShader(const std::string& id) {
		auto itr = s_shaderMap.find(id);
		if (itr != s_shaderMap.end())
			return itr->second;
		else {
			Logger::Writef(LogEntry::Level::Error, "shader {:?} not found", id);
			return nullptr;
		}
	}

	inline static bool AddShader(const std::string& id, const std::shared_ptr<Shader> shader) {
		assert(shader != nullptr);
		bool success = s_shaderMap.insert(std::make_pair(id, shader)).second;
		if (!success)
			Logger::Writef(LogEntry::Level::Error, "failed to add shader {:?}", id);
		return success;
	}

	inline static bool RemoveShader(const std::string& id) {
		bool success = (s_shaderMap.erase(id) == 1);
		if (!success)
			Logger::Writef(LogEntry::Level::Error, "shader {:?} not found", id);
		return success;
	}

	inline static const std::unordered_map<std::string, std::shared_ptr<Shader>>& GetAll() {
		return s_shaderMap;
	}
};

}
