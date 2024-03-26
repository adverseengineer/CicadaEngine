#pragma once
#include "GraphicsObject.h"
#include <optional>

class ObjectManager {

private:
	std::unordered_map<std::string, std::shared_ptr<GraphicsObject>> objectMap;

public:
	inline ObjectManager() = default;
	inline ~ObjectManager() = default;
	
	const std::shared_ptr<GraphicsObject>& GetObject(const std::string& key) const;
	bool AddObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj);
	bool SetObject(const std::string& key, const std::shared_ptr<GraphicsObject>& obj);
	
	void Update(double elapsedSeconds);
};

