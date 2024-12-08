#pragma once

#include "BoundingBox.hpp"
#include "IRenderable.hpp"
#include "Transform.hpp"
#include <memory>

class MeshInstance : public IRenderable {
private:
	Mesh* mesh;
	std::shared_ptr<Material> material;
	BoundingBox boundingBox;
	Transform transform;

	bool dirtyBoundingBox;

	/**
	 * Recalculates the bounding box of the object.
	 * 
	 */
	void recalculateBoundingBox();

	/**
	 * Getter for the bounding box of the object.
	 * 
	 * \return The object's bounding box.
	 */
	const BoundingBox& getBoundingBox();
public:
	/**
	 * Constructor of a mesh instance.
	 * 
	 * \param _name The object's name.
	 * \param _mesh The mesh of the instance.
	 * \param _material The material to draw the instance with.
	 * \param _transform The current object's tranfsorm.
	 */
	MeshInstance(const std::string& _name, Mesh* _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform);

	/**
	 * Gets all the objects that need to be rendered to the screen.
	 *
	 * \return The objects containing: a mesh pointer, a material pointer, the model matrix, the bounding box.
	 */
	std::vector<std::tuple<Mesh*, Material *, glm::mat4, BoundingBox>> getDrawables() override;

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