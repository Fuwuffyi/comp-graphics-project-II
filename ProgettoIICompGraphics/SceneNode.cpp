#include "SceneNode.hpp"

SceneNode::SceneNode(const std::string& _name, const Transform& _transform, const std::shared_ptr<SceneNode>& parent)
	:
	transform(_transform),
	worldTransform(_transform),
	parentNode(parent),
	childNodes(),
	name(_name)
{
	updateWorldTransform();
}

void SceneNode::updateWorldTransform(const glm::mat4& parentModelMatrix) {
	if (!this->parentNode) {
		this->worldTransform = transform;
	} else {
		this->worldTransform = this->parentNode->getWorldTransform() * this->transform;
	}
	// Update child node transforms
	for (std::shared_ptr<SceneNode>& node : this->childNodes) {
		node->updateWorldTransform(this->worldTransform.getTransformMatrix());
	}
}

const Transform& SceneNode::getWorldTransform() const {
	return this->worldTransform;
}

void SceneNode::setPosition(const glm::vec3& newPos) {
	this->transform.setPosition(newPos);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

void SceneNode::setRotation(const glm::vec3& newRot) {
	this->transform.setRotation(newRot);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

void SceneNode::setScale(const glm::vec3& newScale) {
	this->transform.setScale(newScale);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

void SceneNode::changePosition(const glm::vec3& posOffset) {
	this->transform.setPosition(this->transform.getPosition() + posOffset);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

void SceneNode::changeRotation(const glm::vec3& rotOffset) {
	this->transform.setRotation(this->transform.getRotation() + rotOffset);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

void SceneNode::changeScale(const glm::vec3& scaleOffset) {
	this->transform.setScale(this->transform.getScale() + scaleOffset);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

void SceneNode::addChild(const std::shared_ptr<SceneNode>& child) {
	this->childNodes.emplace_back(child);
	this->updateWorldTransform(this->worldTransform.getTransformMatrix());
}

const std::vector<std::shared_ptr<SceneNode>>& SceneNode::getChildren() const {
	return this->childNodes;
}
