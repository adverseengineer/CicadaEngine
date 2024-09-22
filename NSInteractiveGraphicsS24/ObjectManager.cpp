#include "ObjectManager.h"

std::unordered_map<std::string, std::shared_ptr<GameObject>> ObjectManager::s_objectMap;

void ObjectManager::Update(double elapsedSeconds) {
	for (auto& [_, obj] : s_objectMap) {
		obj->Update(elapsedSeconds);
	}
}
