#pragma once

#include "entt/entt.hpp"

namespace Cicada::ECS::Components {

	struct Relationship {
		entt::entity parent = entt::null;
		//TODO: add properties to represent children and maybe siblings
	};

}
