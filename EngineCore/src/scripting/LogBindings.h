#pragma once

#include "Logger.h"
#include <sol/sol.hpp>

namespace Cicada::Scripting {

	inline static void CreateLogBindings(sol::state& lua) {

		const sol::function& tostring = lua["tostring"];
		
		lua["print"] = sol::nil; //un-register print, we don't want it

		lua.set_function("log", [tostring](const sol::variadic_args& args) {
			for (const auto& arg : args) {
				Logger::Log(tostring(arg));
			}
		});

		lua.set_function("warn", [tostring](const sol::variadic_args& args) {
			for (const auto& arg : args) {
				Logger::Warn(tostring(arg));
			}
		});

		lua.set_function("error", [tostring](const sol::variadic_args& args) {
			for (const auto& arg : args) {
				Logger::Error(tostring(arg));
			}
		});
	}

}
