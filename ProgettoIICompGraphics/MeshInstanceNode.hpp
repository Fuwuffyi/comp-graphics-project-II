#pragma once

#include "SceneNode.hpp"
#include "BoundingBox.hpp"

class Mesh;
class Material;

class MeshInstanceNode : public SceneNode {
private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	BoundingBox boundingBox;
protected:
	virtual void updateWorldTransform() override;
public:
	/**
	 * Constructor for a node containing a mesh.
	 * 
	 * \param _name The node's name.
	 * \param _mesh The node's mesh.
	 * \param _material The mesh's material.
	 * \param _transform The node's transform.
	 * \param parent The node's possible parent.
	 */
	MeshInstanceNode(const std::string& _name, const std::shared_ptr<Mesh>& _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform, const std::shared_ptr<SceneNode>& parent = nullptr);

	/**
	 * Getter for the mesh pointer of the node.
	 * 
	 * \return The mesh pointer of the node.
	 */
	Mesh* getMesh() const;

	/**
	 * Getter for the material pointer of the node.
	 *
	 * \return The material pointer of the node.
	 */
	const std::shared_ptr<Material>& getMaterial() const;

	/**
	 * Setter for the material of the node.
	 * 
	 * \param _material The new material for the object.
	 */
	void setMaterial(const std::shared_ptr<Material>& _material);

	/**
	 * Getter for the object's bounding box.
	 *
	 * \return The object's bounding box.
	 */
	const BoundingBox getBoundingBox() const;
};
