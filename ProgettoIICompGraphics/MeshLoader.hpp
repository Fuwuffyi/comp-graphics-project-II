#pragma once

#include "Mesh.hpp"

#include <string>
#include <vector>

namespace MeshLoader {
	std::vector<Mesh3D*> loadMesh(const std::string& fileName);
}