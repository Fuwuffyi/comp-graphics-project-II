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

	virtual void updateWorldTransform(const glm::mat4& parentModelMatrix = glm::mat4(1.0f));
public:
	const std::string name;

	SceneNode(const std::string& _name, const Transform& _transform, const std::shared_ptr<SceneNode>& parent = nullptr);

	const Transform& getWorldTransform() const;
	const Transform& getLocalTransform() const;

	void setPosition(const glm::vec3& newPos);
	void setRotation(const glm::vec3& newRot);
	void setScale(const glm::vec3& newScale);
	void changePosition(const glm::vec3& posOffset);
	void changeRotation(const glm::vec3& rotOffset);
	void changeScale(const glm::vec3& scaleOffset);

	void addChild(const std::shared_ptr<SceneNode>& child);
	const std::vector<std::shared_ptr<SceneNode>>& getChildren() const;
};
