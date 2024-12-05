#pragma once

#include "IRenderable.hpp"
#include "Transform.hpp"

struct Vertex2D;
struct Vertex3D;

template <typename T>
class MeshInstance : public IRenderable<T> {
private:
	Mesh<T>* mesh;
	Material* material;
	Transform transform;
public:
	MeshInstance(Mesh<T>* _mesh, Material* _material, const Transform& _transform);

	std::vector<std::tuple<Mesh<T>*, Material*, Transform*>> getDrawables() override;
};

using MeshInstance2D = MeshInstance<Vertex2D>;
using MeshInstance3D = MeshInstance<Vertex3D>;