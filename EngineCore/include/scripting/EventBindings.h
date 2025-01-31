#pragma once

#include "EventManager.h"
#include <sol/sol.hpp>

namespace Cicada::Scripting {

	inline static void CreateEventBindings(sol::state& lua) {

		sol::table eventTable = lua.create_table("Event");
		eventTable.set_function("Register", &EventManager::RegisterEventCallback);
	
		//TODO: consider actually using this pattern
		/*eventtable.set_function("onstart", [](const sol::function& luacallback) {
			eventmanager::registereventcallback("onstart", luacallback);
		});
		eventtable.set_function("onupdate", [](const sol::function& luacallback) {
			eventmanager::registereventcallback("onupdate", luacallback);
		});*/

	}

}
