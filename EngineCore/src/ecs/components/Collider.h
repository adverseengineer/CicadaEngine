#pragma once

#include <glm/glm.hpp>

namespace Cicada::ECS::Components {

	//TODO: for now its just aabb
	struct Collider {
		glm::vec3 start;
		glm::vec3 end;
	};

}
