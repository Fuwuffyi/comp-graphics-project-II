#include "ModelInstance.hpp"

#include "Mesh.hpp"

ModelInstance::ModelInstance(const std::vector<MeshInstance>& _meshInstances, const Transform& _transform)
	:
	meshInstances(_meshInstances),
	transform(_transform)
{}

std::vector<std::tuple<Mesh*, Material *, glm::mat4, BoundingBox>> ModelInstance::getDrawables() {
	std::vector<std::tuple<Mesh*, Material*, glm::mat4, BoundingBox>> drawables;
	for (MeshInstance& instance : this->meshInstances) {
		std::vector<std::tuple<Mesh*, Material *, glm::mat4, BoundingBox>> instanceDrawables = instance.getDrawables();
		for (auto& [meshPtr, materialPtr, modelMatrix, aabb] : instanceDrawables) {
			modelMatrix = this->transform.getTransformMatrix() * instance.getMutableTransform().getTransformMatrix();
			aabb = meshPtr->getBoundingBox().transform(modelMatrix);
		}
		drawables.insert(drawables.end(), instanceDrawables.begin(), instanceDrawables.end());
	}
	return drawables;
}

const Transform& ModelInstance::getTransform() const {
	return this->transform;
}

Transform& ModelInstance::getMutableTransform() {
	return this->transform;
}