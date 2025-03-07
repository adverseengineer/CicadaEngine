#pragma once

#include "core/Mesh.h"

namespace Cicada::ECS {

	struct MeshComponent {
		//TODO: reconsider how we store the mesh here
		std::shared_ptr<Cicada::Mesh> ptr = nullptr;
	};

}
