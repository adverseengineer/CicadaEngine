#include "ObjectManager.h"

const std::shared_ptr<GraphicsObject>& ObjectManager::GetObject(const std::string& key) const {
	//TODO: handle the possible out_of_range exception here
	return objectMap.at(key);
}

bool ObjectManager::AddObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj) {
	const auto& result = objectMap.insert({key, obj});
	return result.second;
}

bool ObjectManager::SetObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj) {
	if(objectMap.contains(key)) {
		objectMap.at(key) = obj;
		return true;
	}
	else
		return false;
}

void ObjectManager::Update(double elapsedSeconds) {
	for (auto& [key, obj] : objectMap) {
		obj->Update(elapsedSeconds);
	}
}
