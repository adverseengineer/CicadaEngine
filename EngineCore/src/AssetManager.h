#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Asset.h"

namespace Cicada {

class AssetManager {

	static std::unordered_map<std::string, std::weak_ptr<Asset>> s_assetCache;

	template <typename T>
	static std::shared_ptr<Asset<T>> Create(std::string_view name, Args&&... args) {
		auto it = s_assetCache.find(name.data());
		if (it != s_assetCache.end()) {
			LOG_WARN("Instance already exists: {:?}", name);
			return it->second.lock();
		}
		auto instance = std::shared_ptr<T>(new T(name, std::forward<Args>(args)...));
		s_assetCache.emplace(name.data(), instance);
		return instance;
	}
};

}
