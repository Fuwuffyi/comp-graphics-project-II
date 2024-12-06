#include "ModelInstance.hpp"

template <typename T>
ModelInstance<T>::ModelInstance(const std::vector<MeshInstance<T>>& _meshInstances, const Transform& _transform)
	:
	meshInstances(_meshInstances),
	transform(_transform)
{}

template <typename T>
std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>> ModelInstance<T>::getDrawables() {
	std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>> drawables;
	for (MeshInstance<T>& instance : this->meshInstances) {
		std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>> instanceDrawables = instance.getDrawables();
		for (auto& [meshPtr, materialPtr, modelMatrix] : instanceDrawables) {
			modelMatrix = this->transform.getTransformMatrix() * instance.getMutableTransform().getTransformMatrix();
		}
		drawables.insert(drawables.end(), instanceDrawables.begin(), instanceDrawables.end());
	}
	return drawables;
}

template <typename T>
const Transform& ModelInstance<T>::getTransform() const {
	return this->transform;
}

template <typename T>
Transform& ModelInstance<T>::getMutableTransform() {
	return this->transform;
}

template class ModelInstance<Vertex2D>;
template class ModelInstance<Vertex3D>;