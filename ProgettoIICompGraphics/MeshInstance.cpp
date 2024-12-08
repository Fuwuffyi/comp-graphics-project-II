#include "MeshInstance.hpp"

#include "Mesh.hpp"
#include "Transform.hpp"

MeshInstance::MeshInstance(Mesh* _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform)
	:
	mesh(_mesh),
	material(_material),
	transform(_transform),
	boundingBox(this->mesh->getBoundingBox()),
	dirtyBoundingBox(true)
{}

void MeshInstance::recalculateBoundingBox() {
	this->boundingBox = this->mesh->getBoundingBox().transform(this->transform.getTransformMatrix());
}

const BoundingBox& MeshInstance::getBoundingBox() {
	if (this->dirtyBoundingBox) {
		this->recalculateBoundingBox();
		this->dirtyBoundingBox = false;
	}
	return this->boundingBox;
}

std::vector<std::tuple<Mesh *, Material *, glm::mat4, BoundingBox>> MeshInstance::getDrawables() {
	return std::vector<std::tuple<Mesh *, Material *, glm::mat4, BoundingBox>>({ std::make_tuple(this->mesh, this->material.get(), this->transform.getTransformMatrix(), this->getBoundingBox())});
};

const Transform& MeshInstance::getTransform() const {
	return this->transform;
}

Transform& MeshInstance::getMutableTransform() {
	this->dirtyBoundingBox = true;
	return this->transform;
}