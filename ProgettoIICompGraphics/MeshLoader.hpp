#pragma once

#include <string>
#include <vector>

class Mesh;

namespace MeshLoader {
	std::vector<Mesh *> loadMesh(const std::string& fileName);
}