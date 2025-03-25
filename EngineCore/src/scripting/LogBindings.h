#pragma once

#include "Log.h"
#include <sol/sol.hpp>

namespace Cicada::Scripting {

	inline static void CreateLogBindings(sol::state& lua) {

		const sol::function& tostring = lua["tostring"];
		
		lua["print"] = sol::nil; //un-register print, we don't want it

		//TODO: expose all new logging functions

		lua.set_function("log", [tostring](const sol::variadic_args& args) {
			for (const auto& arg : args) {
				//Log::Log(tostring(arg));
				//TODO: figure out how to convert this
			}
		});

		lua.set_function("warn", [tostring](const sol::variadic_args& args) {
			for (const auto& arg : args) {
				//Log::Warn(tostring(arg));
				//TODO: figure out how to convert this
			}
		});

		lua.set_function("error", [tostring](const sol::variadic_args& args) {
			for (const auto& arg : args) {
				//Log::Error(tostring(arg));
				//TODO: figure out how to convert this
			}
		});
	}

}
