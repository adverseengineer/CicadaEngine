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

	static int panic(lua_State*) {
		Log::Error("PANIC!!");
		return 0;
	}

public:
	inline static void Init() {
		//set up the script environment for the entire state of the engine
		s_lua.open_libraries(sol::lib::base);

		s_lua.set_panic(panic);

		Scripting::CreateLogBindings(s_lua);
		Scripting::CreateEventBindings(s_lua);
		Scripting::CreateMathBindings(s_lua);
		Scripting::CreateInputBindings(s_lua);
		Scripting::CreateEntityBindings(s_lua);
	};

	inline static void LoadScript(const std::string& path) {

		sol::environment env(s_lua, sol::create, s_lua.globals());
		try {
			s_lua.safe_script_file(path, env);
		}
		catch (sol::error e) {
			//Log::Error(e.what());
			//TODO: why can't spdlog handle a cstr?
		}
	}
};

}
