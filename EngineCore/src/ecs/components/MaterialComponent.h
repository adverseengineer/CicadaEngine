#pragma once

#include "core/Material.h"
#include <memory>

namespace Cicada::ECS::Components {

	struct MaterialComponent {
		//TODO: reconsider how this is referenced/stored
		std::shared_ptr<Cicada::Material> ptr;
	};

}
