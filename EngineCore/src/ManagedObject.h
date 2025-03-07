#pragma once

#include "Logger.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Cicada {

template <typename T>
class ManagedObject {
private:
	inline static std::unordered_map<std::string, std::weak_ptr<T>> s_instances;

protected:
	std::string m_name;

	ManagedObject(std::string_view name) : m_name(name) {
		Logger::Writef(LogEntry::Level::Info, "Creating instance: {:?}", name);
	}

public:
	//takes a name for the new instance and any parameters needed for the derived class constructor
	template <typename... Args>
	static std::shared_ptr<T> Create(std::string_view name, Args&&... args) {
		auto instance = std::shared_ptr<T>(new T(name, std::forward<Args>(args)...));
		s_instances.emplace(name.data(), instance);
		return instance;
	}

	static std::shared_ptr<T> Get(std::string_view name) {
		auto it = s_instances.find(name.data());
		return (it != s_instances.end()) ? it->second.lock() : nullptr;
	}

	virtual ~ManagedObject() {
		Logger::Writef(LogEntry::Level::Info, "Destroying instance: {:?}", static_cast<T*>(this)->m_name);
		s_instances.erase(static_cast<T*>(this)->m_name);
	}

	static void ForEach(std::function<void(std::shared_ptr<T>)> callback) {
		for (auto& [_, ptr] : s_instances)
			callback(ptr.lock());
	}
};

}
