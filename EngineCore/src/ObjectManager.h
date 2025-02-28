#pragma once

#include "GameObject.h"
#include "Logger.h"

namespace Cicada {

class ObjectManager {

private:
	ObjectManager() = delete;
	static std::unordered_map<std::string, std::shared_ptr<GameObject>> s_objectMap;

public:
	inline static std::shared_ptr<GameObject> GetObject(const std::string& id) {
		auto itr = s_objectMap.find(id);
		if (itr != s_objectMap.end())
			return itr->second;
		else {
			Logger::Writef(LogEntry::Level::Warning, "object {:?} not found", id);
			return nullptr;
		}
	}

	inline static bool AddObject(const std::string& id, const std::shared_ptr<GameObject> object) {
		assert(object != nullptr);
		bool success = s_objectMap.insert(std::make_pair(id, object)).second;
		if (!success)
			Logger::Writef(LogEntry::Level::Warning, "failed to add object {:?}", id);
		return success;
	}

	inline static bool RemoveObject(const std::string& id) {
		bool success = (s_objectMap.erase(id) == 1);
		if (!success)
			Logger::Writef(LogEntry::Level::Warning, "object {:?} not found", id);
		return success;
	}

	inline static const std::unordered_map<std::string, std::shared_ptr<GameObject>>& GetAll() {
		return s_objectMap;
	}

	static void Update(double elapsedSeconds);
};

}
