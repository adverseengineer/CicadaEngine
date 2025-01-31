#pragma once
#define SOL_ALL_SAFETIES_ON 1
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1

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
	static std::unordered_map<std::string, std::vector<sol::function>> s_callbackMap;	

public:
	static void RegisterEventCallback(const std::string& eventType, const sol::function& luaCallback) {
		s_callbackMap[eventType].push_back(luaCallback);
	}

	//to be called from c++ code in the game loop, not to be exposed to lua
	//TODO: this will need to be reworked when i implement events that need parameters like input or deltatime
	static void TriggerEvent(const std::string& eventType) {
		
		auto it = s_callbackMap.find(eventType);
		if (it != s_callbackMap.end()) {
			for (const auto& callback : it->second) {
				callback();
			}
		}
	}
};

}
