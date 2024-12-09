#include "SceneNode.hpp"

SceneNode::SceneNode(const std::string& _name, const Transform& _transform, const std::shared_ptr<SceneNode>& parent)
	:
	localTransform(_transform),
	worldTransform(_transform),
	parentNode(parent),
	childNodes(),
	name(_name)
{
	updateWorldTransform();
}

void SceneNode::updateWorldTransform() {
	if (!this->parentNode) {
		this->worldTransform = localTransform;
	} else {
		this->worldTransform = this->parentNode->getWorldTransform() * this->localTransform;
	}
	// Update child node transforms
	for (std::shared_ptr<SceneNode>& node : this->childNodes) {
		node->updateWorldTransform();
	}
}

const Transform& SceneNode::getWorldTransform() const {
	return this->worldTransform;
}

const Transform& SceneNode::getLocalTransform() const {
	return this->localTransform;
}

void SceneNode::setPosition(const glm::vec3& newPos) {
	this->localTransform.setPosition(newPos);
	this->updateWorldTransform();
}

void SceneNode::setRotation(const glm::vec3& newRot) {
	this->localTransform.setRotation(newRot);
	this->updateWorldTransform();
}

void SceneNode::setScale(const glm::vec3& newScale) {
	this->localTransform.setScale(newScale);
	this->updateWorldTransform();
}

void SceneNode::changePosition(const glm::vec3& posOffset) {
	this->localTransform.setPosition(this->localTransform.getPosition() + posOffset);
	this->updateWorldTransform();
}

void SceneNode::changeRotation(const glm::vec3& rotOffset) {
	this->localTransform.setRotation(this->localTransform.getRotation() + rotOffset);
	this->updateWorldTransform();
}

void SceneNode::changeScale(const glm::vec3& scaleOffset) {
	this->localTransform.setScale(this->localTransform.getScale() + scaleOffset);
	this->updateWorldTransform();
}

const std::shared_ptr<SceneNode>& SceneNode::getParent() const {
	return this->parentNode;
}

void SceneNode::setParent(const std::shared_ptr<SceneNode>& newParent) {
	this->parentNode = newParent;
	this->updateWorldTransform();
}

void SceneNode::addChild(const std::shared_ptr<SceneNode>& child) {
	this->childNodes.emplace_back(child);
	child->updateWorldTransform();
}

const std::vector<std::shared_ptr<SceneNode>>& SceneNode::getChildren() const {
	return this->childNodes;
}
