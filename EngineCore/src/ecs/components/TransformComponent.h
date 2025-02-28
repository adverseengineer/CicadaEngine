#pragma once

#include <glm/glm.hpp>

namespace Cicada::ECS::Components {
	
	//TODO: consider combining hierarchical relationships with transform data, because you can't have one without the other
	//what if theres an object in the hierarchy with no transform? completely screws up global transform calculation
	struct TransformComponent {
		glm::vec4 localTransform;
	};

}
