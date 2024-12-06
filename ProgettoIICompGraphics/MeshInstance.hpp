#pragma once

#include "IRenderable.hpp"
#include "Transform.hpp"

struct Vertex2D;
struct Vertex3D;

class MeshInstance : public IRenderable {
private:
	Mesh* mesh;
	Material* material;
	Transform transform;
public:
	MeshInstance(Mesh* _mesh, Material* _material, const Transform& _transform);

	std::vector<std::tuple<Mesh*, Material*, glm::mat4>> getDrawables() override;

	const Transform& getTransform() const;
	Transform& getMutableTransform();
};