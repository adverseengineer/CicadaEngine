#include "ShaderManager.h"

using namespace Cicada;

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::s_shaderMap;
