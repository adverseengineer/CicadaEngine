#pragma once

#include "GraphicsObject.h"
#include <optional>

class ObjectManager {

private:
	std::unordered_map<std::string, std::shared_ptr<GraphicsObject>> objectMap;

public:
	ObjectManager() = default;
	~ObjectManager() = default;
	
	void SetObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj);
	std::shared_ptr<GraphicsObject> GetObject(const std::string& key) const;

	void Update(double elapsedSeconds);
};

