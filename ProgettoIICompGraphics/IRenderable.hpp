#pragma once

#include <glm/glm.hpp>
#include <vector>

class Transform;
class Material;

template <typename T>
class Mesh;

template <typename T>
class IRenderable {
	virtual std::vector<std::tuple<Mesh<T>*, Material*, glm::mat4>> getDrawables() = 0;
};