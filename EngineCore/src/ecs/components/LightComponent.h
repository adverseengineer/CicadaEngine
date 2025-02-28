#pragma once

#include <glm/glm.hpp>

namespace Cicada::ECS::Components {

	struct LightComponent {
		glm::vec3 position;
		glm::vec3 color;
		float intensity;
		float attenuationCoef;
	};

}
