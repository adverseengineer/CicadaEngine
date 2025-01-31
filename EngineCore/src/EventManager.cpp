
#include "EventManager.h"

using namespace Cicada;

std::unordered_map<std::string, std::vector<sol::function>> EventManager::s_callbackMap;
