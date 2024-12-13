#pragma once

#include "Transform.hpp"
#include <string>
#include <memory>

class SceneNode {
protected:
	Transform localTransform; // Local transform relative to the parent
	Transform worldTransform; // Transform relative to the world

	std::shared_ptr<SceneNode> parentNode;
	std::vector<std::shared_ptr<SceneNode>> childNodes;

	/**
	 * Updates its and all of its children's world transforms..
	 * 
	 */
	virtual void updateWorldTransform();
public:
	std::string name;

	/**
	 * Constructor for a basic scene node.
	 * 
	 * \param _name The name of the node.
	 * \param _transform The transform of the node.
	 * \param parent A possible parent for this node.
	 */
	SceneNode(const std::string& _name, const Transform& _transform, const std::shared_ptr<SceneNode>& parent = nullptr);

	/**
	 * Getter for the world transform of the object.
	 * 
	 * \return The world transform of the object.
	 */
	const Transform& getWorldTransform() const;

	/**
	 * Getter for the local transform of the object relative to the parent.
	 *
	 * \return The local transform of the object.
	 */
	const Transform& getLocalTransform() const;

	/**
	 * Sets the object's local position.
	 * 
	 * \param newPos The new local position.
	 */
	void setPosition(const glm::vec3& newPos);

	/**
	 * Sets the object's local rotation.
	 *
	 * \param newRot The new local rotation.
	 */
	void setRotation(const glm::vec3& newRot);

	/**
	 * Sets the object's local scale.
	 *
	 * \param newScale The new local scale.
	 */
	void setScale(const glm::vec3& newScale);

	/**
	 * Changes the object's local position.
	 *
	 * \param posOffset The offset of the original position.
	 */
	void changePosition(const glm::vec3& posOffset);

	/**
	 * Changes the object's local rotation.
	 *
	 * \param rotOffset The offset of the original rotation.
	 */
	void changeRotation(const glm::vec3& rotOffset);

	/**
	 * Changes the object's local scale.
	 *
	 * \param scaleOffset The offset of the original scale.
	 */
	void changeScale(const glm::vec3& scaleOffset);

	/**
	 * Sets the current object's parent.
	 * 
	 * \param newParent The new parent of the object.
	 */
	void setParent(const std::shared_ptr<SceneNode>& newParent);
	
	/**
	 * Gets the node's parent.
	 * 
	 * \return The parent of the node.
	 */
	const std::shared_ptr<SceneNode>& getParent() const;

	/**
	 * Adds a child to the node.
	 * 
	 * \param child The child to add to the node.
	 */
	void addChild(const std::shared_ptr<SceneNode>& child);
	
	/**
	 * Gets all the children in the current node.
	 * 
	 * \return The children of the current node.
	 */
	const std::vector<std::shared_ptr<SceneNode>>& getChildren() const;
};
