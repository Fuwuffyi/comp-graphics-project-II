#include "ModelInstance.hpp"

template <typename T>
ModelInstance<T>::ModelInstance(const std::vector<MeshInstance<T>>& _meshInstances) 
	:
	meshInstances(_meshInstances)
{}

template <typename T>
std::vector<std::tuple<Mesh<T>*, Material*, const glm::mat4&>> ModelInstance<T>::getDrawables() {
	std::vector<std::tuple<Mesh<T>*, Material*, const glm::mat4&>> drawables;
	for (MeshInstance& instance : this->meshInstances) {
		const std::vector<std::tuple<Mesh<T>*, Material*, const glm::mat4&>> instanceDrawables = instance.getDrawables();
		drawables.insert(drawables.end(), instanceDrawables.begin(), instanceDrawables.end());
	}
	return drawables;
}