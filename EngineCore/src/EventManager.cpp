
#include <EventManager.h>

std::unordered_map<std::string, std::vector<sol::function>> EventManager::s_events;
sol::state EventManager::s_lua;
