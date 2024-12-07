#include "MeshInstance.hpp"

#include "Transform.hpp"
#include "Mesh.hpp"

MeshInstance::MeshInstance(Mesh* _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform)
	:
	mesh(_mesh),
	material(_material),
	transform(_transform)
{}

std::vector<std::tuple<Mesh *, Material *, glm::mat4>> MeshInstance::getDrawables() {
	return std::vector<std::tuple<Mesh *, Material *, glm::mat4>>({ std::make_tuple(this->mesh, this->material.get(), this->transform.getTransformMatrix())});
};

const Transform& MeshInstance::getTransform() const {
	return this->transform;
}

Transform& MeshInstance::getMutableTransform() {
	return this->transform;
}