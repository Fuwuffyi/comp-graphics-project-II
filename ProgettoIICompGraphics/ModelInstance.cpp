#include "ModelInstance.hpp"

#include "Mesh.hpp"

ModelInstance::ModelInstance(const std::vector<MeshInstance>& _meshInstances, const Transform& _transform)
	:
	meshInstances(_meshInstances),
	transform(_transform)
{}

std::vector<std::tuple<Mesh*, Material *, glm::mat4, BoundingBox>> ModelInstance::getDrawables() {
	std::vector<std::tuple<Mesh*, Material*, glm::mat4, BoundingBox>> drawables;
	// Check all sub instances of this parent
	for (MeshInstance& instance : this->meshInstances) {
		std::vector<std::tuple<Mesh*, Material *, glm::mat4, BoundingBox>> instanceDrawables = instance.getDrawables();
		// Add their values to the return vector
		for (auto& [meshPtr, materialPtr, modelMatrix, aabb] : instanceDrawables) {
			// Calculate new model matrices and bounding boxes based on the transform of the parent
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