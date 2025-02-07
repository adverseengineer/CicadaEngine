#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/norm.hpp>

namespace Cicada::Scripting {

	void CreateMathBindings(sol::state& lua);
}
