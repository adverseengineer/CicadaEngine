#pragma once

#include "Logger.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Cicada {

template <typename T>
class ManagedObject {
public:
	//takes a name for the new instance and any parameters needed for the derived class constructor
	template <typename... Args>
	static std::shared_ptr<T> Create(std::string_view name, Args&&... args) {
		auto instance = std::shared_ptr<T>(new T(name, std::forward<Args>(args)...));
		GetInstances()[name.data()] = instance;
		return instance;
	}

	static std::shared_ptr<T> Get(std::string_view name) {
		auto it = GetInstances().find(name.data());
		return (it != GetInstances().end()) ? it->second.lock() : nullptr;
	}

	virtual ~ManagedObject() {
		Logger::Writef(LogEntry::Level::Info, "Destroying instance: {:?}", static_cast<T*>(this)->m_name);
		GetInstances().erase(static_cast<T*>(this)->m_name);
	}

	static void ForEach(std::function<void(std::shared_ptr<T>)> callback) {
		for (auto& [_, ptr] : GetInstances())
			callback(ptr.lock());
	}

protected:
	std::string m_name;

	ManagedObject(std::string_view name) : m_name(name) {
		Logger::Writef(LogEntry::Level::Info, "Creating instance: {:?}", name);
	}

private:
	static std::unordered_map<std::string, std::weak_ptr<T>>& GetInstances() {
		static std::unordered_map<std::string, std::weak_ptr<T>> instances;
		return instances;
	}
};

}
