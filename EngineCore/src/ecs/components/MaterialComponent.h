#pragma once

#include "core/Material.h"

namespace Cicada::ECS {

	struct MaterialComponent {
		std::shared_ptr<Cicada::Material> ptr;
		//TODO: add other fields for settings, etc.
	};

}
