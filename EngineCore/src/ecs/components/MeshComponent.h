#pragma once

#include "core/Mesh.h"
#include <memory>

namespace Cicada::ECS::Components {

	struct MeshComponent {
		//TODO: reconsider how we store the mesh here
		std::shared_ptr<Cicada::Mesh> ptr = nullptr;
	};

}
