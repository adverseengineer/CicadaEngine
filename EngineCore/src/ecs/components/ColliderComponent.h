#pragma once

#include <glm/glm.hpp>

namespace Cicada::ECS {

	//TODO: for now its just aabb
	struct ColliderComponent {
		glm::vec3 start;
		glm::vec3 end;
	};

}
