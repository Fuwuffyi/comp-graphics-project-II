#pragma once

#include <glm/glm.hpp>
#include <vector>

class Mesh;
class Material;

class RenderingQueue {
private:
	std::vector<std::tuple<Mesh*, Material*, glm::mat4>> renderables;

	const bool closestFirst;
public:
	RenderingQueue(const bool _closestFirst = true);

	void addRenderable(Mesh* mesh, Material* material, const glm::mat4& modelMatrix);
	void render(const glm::mat4& cameraMatrix, const glm::vec3& viewPoint);
	void clear();
};
