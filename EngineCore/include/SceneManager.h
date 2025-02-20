#pragma once

#include "GameObject.h"
#include "GraphicsStructures.h"
#include <memory>
#include <stack>
#include <unordered_map>

#include <unordered_set>

namespace Cicada {

class SceneManager {
private:
	
	std::shared_ptr<GameObject> m_sceneGraphRoot;
	std::unordered_map<std::string, Light> m_lights;

	std::unordered_set<std::shared_ptr<GameObject>> objects; //TODO: keep for now while switching over

	SceneManager() = default;
	~SceneManager() = default;
public:

	inline static SceneManager& Instance() {
		static SceneManager instance;
		return instance;
	}

	class SceneView; //TODO: implement this, and do it like an iterator

	template <typename ComponentType>
	SceneView GetObjectsByComponent() = delete; //TODO: implement
	SceneView GetObjectsByTag(std::string_view tag) = delete; //TODO: implement
	
	std::optional<GameObject> GetObjectByID(std::string_view id) = delete; //TODO: implement
	std::optional<GameObject> CreateObject() = delete; //TODO: implement, consider what params it needs
	bool DestroyObject(std::string_view id) = delete; //TODO: implement

	bool LoadSceneNode(std::string_view path) = delete; //TOOD: implement
	
	//TODO: implement foreach traversal of scene graph

	inline const std::unordered_set<std::shared_ptr<GameObject>>& GetObjects() const { return objects; }
	inline void AddObject(const std::shared_ptr<GameObject>& object) { objects.insert(object); }
};

class SceneIterator {

using Node = std::shared_ptr<GameObject>;
private:
	std::stack<Node> m_stack;

public:
	inline SceneIterator(Node& root) {
		if (root != nullptr) m_stack.push(root);
	}

	bool hasNext() const {
		return !m_stack.empty();
	}

	Node next() {
		if (m_stack.empty()) throw std::out_of_range("no elements left");

		auto& node = m_stack.top();
		m_stack.pop();

		for (auto& child : node->GetChildren()) {
			
		}
	}
};

}