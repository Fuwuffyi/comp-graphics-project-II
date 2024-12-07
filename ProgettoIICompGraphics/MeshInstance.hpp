#pragma once

#include "IRenderable.hpp"
#include "Transform.hpp"

#include <memory>

struct Vertex2D;
struct Vertex3D;

class MeshInstance : public IRenderable {
private:
	Mesh* mesh;
	std::shared_ptr<Material> material;
	Transform transform;
public:
	MeshInstance(Mesh* _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform);

	std::vector<std::tuple<Mesh*, Material *, glm::mat4>> getDrawables() override;

	const Transform& getTransform() const;
	Transform& getMutableTransform();
};