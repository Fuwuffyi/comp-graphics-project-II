#include "MeshInstance.hpp"

#include "Transform.hpp"
#include "Mesh.hpp"

template <typename T>
MeshInstance<T>::MeshInstance(Mesh<T>* _mesh, Material* _material, const Transform& _transform)
	:
	mesh(_mesh),
	material(_material),
	transform(_transform)
{}

template <typename T>
std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>> MeshInstance<T>::getDrawables() {
	return std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>>({ std::make_tuple(this->mesh, this->material, this->transform.getTransformMatrix()) });
};

template class MeshInstance<Vertex2D>;
template class MeshInstance<Vertex3D>;