#pragma once

#include "MeshInstance.hpp"

struct Vertex2D;
struct Vertex3D;

template <typename T>
class ModelInstance : public IRenderable<T> {
private:
	std::vector<MeshInstance<T>> meshInstances;
public:
	ModelInstance(const std::vector<MeshInstance<T>>& _meshInstances);

	std::vector<std::tuple<Mesh<T>*, Material*, const glm::mat4&>> getDrawables() override;
};

using ModelInstance2D = ModelInstance<Vertex2D>;
using ModelInstance3D = ModelInstance<Vertex3D>;