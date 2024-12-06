#pragma once

#include "MeshInstance.hpp"

struct Vertex2D;
struct Vertex3D;

template <typename T>
class ModelInstance : public IRenderable<T> {
private:
	std::vector<MeshInstance<T>> meshInstances;
	Transform transform;
public:
	ModelInstance(const std::vector<MeshInstance<T>>& _meshInstances, const Transform& _transform);

	std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>> getDrawables() override;

	const Transform& getTransform() const;
	Transform& getMutableTransform();
};

using ModelInstance2D = ModelInstance<Vertex2D>;
using ModelInstance3D = ModelInstance<Vertex3D>;