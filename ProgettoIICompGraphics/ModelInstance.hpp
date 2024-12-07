#pragma once

#include "MeshInstance.hpp"

struct Vertex2D;
struct Vertex3D;

class ModelInstance : public IRenderable {
private:
	std::vector<MeshInstance> meshInstances;
	Transform transform;
public:
	ModelInstance(const std::vector<MeshInstance>& _meshInstances, const Transform& _transform);

	std::vector<std::tuple<Mesh *, Material *, glm::mat4>> getDrawables() override;

	const Transform& getTransform() const;
	Transform& getMutableTransform();
};