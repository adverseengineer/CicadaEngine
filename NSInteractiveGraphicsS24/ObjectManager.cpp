#include "ObjectManager.h"

void ObjectManager::SetObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj) {
	objectMap.insert_or_assign(key, obj);
}

std::shared_ptr<GraphicsObject> ObjectManager::GetObject(const std::string& key) const {
	assert(objectMap.contains(key)); //TODO: remove this at some point and handle it
	return objectMap.at(key);
}

void ObjectManager::Update(double elapsedSeconds) {
	
	for (auto& pair : objectMap) {
		pair.second->Update(elapsedSeconds);
	}
}
