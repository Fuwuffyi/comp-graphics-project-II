#pragma once

#include "MeshInstance.hpp"

class ModelInstance : public IRenderable {
private:
	std::vector<MeshInstance> meshInstances;
	Transform transform;
public:
	/**
	 * Constructor of a model instance.
	 *
	 * \param _name The object's name.
	 * \param _meshInstances The mesh instances that make up this model.
	 * \param _transform The parent's tranfsorm.
	 */
	ModelInstance(const std::string& _name, const std::vector<MeshInstance>& _meshInstances, const Transform& _transform);

	/**
	 * Gets all the objects that need to be rendered to the screen.
	 *
	 * \return The objects containing: a mesh pointer, a material pointer, the model matrix, the bounding box.
	 */
	std::vector<std::tuple<Mesh *, Material *, glm::mat4, BoundingBox>> getDrawables() override;

	/**
	 * Getter for the instance's transform.
	 *
	 * \return The instance's transform.
	 */
	const Transform& getTransform() const;

	/**
	 * Getter for the instance's mutable transform.
	 *
	 * \return The instance's mutable transform.
	 */
	Transform& getMutableTransform();
};