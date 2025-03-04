#pragma once

#include "core/Material.h"
#include "ecs/components/component.h"
#include <memory>

namespace Cicada::ECS::Components {

	struct Material : Component {
		std::shared_ptr<Cicada::Material> ptr;
		//TODO: add other fields for settings, etc.
	};

}
