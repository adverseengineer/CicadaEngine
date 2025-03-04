#pragma once

#include <glm/glm.hpp>

namespace Cicada::ECS::Components {

	struct Light {
		glm::vec3 position;
		glm::vec3 color;
		float intensity;
		float attenuationCoef;
	};

}
