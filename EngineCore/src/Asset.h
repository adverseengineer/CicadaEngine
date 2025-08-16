#pragma once

#include "Log.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace Cicada {

	class Material;
	class Model;
	class Shader;
	class Texture;	

	template <typename T>
	class Asset {
	private:
		inline static std::unordered_map<std::string, std::weak_ptr<T>> s_instances;
		inline static bool DoStuff() {
			YAML::Node node;
		}

	protected:
		std::string m_name;

		Asset(std::string_view name) : m_name(name) {
			LOG_DEBUG("Creating instance: {:?}", name);
		}

	public:
		template <typename... Args>
		static std::shared_ptr<T> Create(std::string_view name, Args&&... args) {
			auto it = s_instances.find(name.data());
			if (it != s_instances.end()) {
				LOG_WARN("Instance already exists: {:?}", name);
				return it->second.lock();
			}
			auto instance = std::shared_ptr<T>(new T(name, std::forward<Args>(args)...));
			s_instances.emplace(name.data(), instance);
			return instance;
		}

		static std::shared_ptr<T> Get(std::string_view name) {
			auto it = s_instances.find(name.data());
			if (it == s_instances.end()) {
				Log::Error("No such instance: {:?}", name);
				return nullptr;
			}
			else
				return it->second.lock();
		}

		virtual ~Asset() {
			LOG_DEBUG("Destroying instance: {:?}", static_cast<T*>(this)->m_name);
			assert(s_instances.contains(static_cast<T*>(this)->m_name));
			s_instances.erase(static_cast<T*>(this)->m_name);
		}

		static void ForEach(std::function<void(std::shared_ptr<T>)> callback) {
			for (auto& [_, ptr] : s_instances)
				callback(ptr.lock());
		}
	};

}
