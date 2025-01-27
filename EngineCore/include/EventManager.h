#pragma once
#define SOL_ALL_SAFETIES_ON 1
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1

#include "Log.h"
#include <glm/vec2.hpp>
#include <sol/sol.hpp>
#include <unordered_map>

//enum class EventType {
//	FrameUpdate, //triggered every time we have a new frame
//	FixedUpdate, //triggered at a constant interval, independent of framerate
//	Load, //triggered when the script or object holding the script is loaded
//	Unload, //triggered when the script or object holding the script is unloaded
//	CollisionEnter, //triggered the frame the attached object's hitbox begins intersecting another
//	CollisionExit, //triggered the frame the attached object's hitbox ceases to intersect another
//	KeyDown, //triggered the frame a key begins being held down
//	KeyUp //tiggered the frame a key ceases to be held down
//};

namespace Cicada {

class EventManager {
private:
	static std::unordered_map<std::string, std::vector<sol::function>> s_events;	
	static sol::state s_lua;

public:
	static void Init() {
		//set up the script environment for the entire state of the engine
		s_lua.open_libraries(sol::lib::base);

		sol::table eventTable = s_lua.create_table("Event");
		eventTable.set_function("Register", &EventManager::RegisterEventCallback);

		//s_lua.set_function("print", sol::nil); //un-register print
		sol::table logTable = s_lua.create_table("log");
		logTable.set_function("info", &Log::Info);
		logTable.set_function("warn", &Log::Warn);
		logTable.set_function("error", &Log::Error);
	};

	static void RegisterEventCallback(std::string eventType, sol::function luaCallback) {
		s_events[eventType].push_back(luaCallback);
	}

	//to be called from c++ code in the game loop, not to be exposed to lua
	//TODO: this will need to be reworked when i implement events that need parameters like input or deltatime
	static void TriggerEvent(const std::string& eventType) {
		
		auto it = s_events.find(eventType);
		if (it != s_events.end()) {
			for (const auto& callback : it->second) {
				callback();
			}
		}
	}

	//TODO: find a better place to put this function, EventManager doesn't feel right
	static void LoadScript(std::string path) {
	
		sol::environment env(s_lua, sol::create, s_lua.globals());
		try {
			s_lua.safe_script_file(path, env);
			//s_lua.script(source);
		}
		catch(sol::error e) {
			Log::Write(LogEntry::Severity::Error, e.what());
		}
	}
};

}
