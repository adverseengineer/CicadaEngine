#pragma once

#include "scripting/EntityBindings.h"
#include "scripting/EventBindings.h"
#include "scripting/InputBindings.h"
#include "scripting/LogBindings.h"
#include "scripting/MathBindings.h"

namespace Cicada {

class ScriptManager {

private:
	static sol::state s_lua;

public:
	inline static void Init() {
		//set up the script environment for the entire state of the engine
		s_lua.open_libraries(sol::lib::base);

		Scripting::CreateLogBindings(s_lua);
		Scripting::CreateEventBindings(s_lua);

		Scripting::CreateVec2Bindings(s_lua);
		Scripting::CreateVec3Bindings(s_lua);
		Scripting::CreateVec4Bindings(s_lua);

		Scripting::CreateInputBindings(s_lua);
		Scripting::CreateEntityBindings(s_lua);

	};

	inline static void LoadScript(const std::string& path) {

		sol::environment env(s_lua, sol::create, s_lua.globals());
		try {
			s_lua.safe_script_file(path, env);
		}
		catch (sol::error e) {
			Logger::Error(e.what());
		}
	}
};

}
