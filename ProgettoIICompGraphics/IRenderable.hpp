#pragma once

#include <glm/glm.hpp>
#include <vector>

class BoundingBox;
class Transform;
class Material;

class Mesh;

class IRenderable {
public:
	virtual std::vector<std::tuple<Mesh *, Material *, glm::mat4, BoundingBox>> getDrawables() = 0;
};