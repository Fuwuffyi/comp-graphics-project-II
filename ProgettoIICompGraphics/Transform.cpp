#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Transform::Transform(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale) 
	:
	matPosition(1.0f),
	matRotation(1.0f),
	matScale(1.0f),
	matTransform(1.0f),
	position(_position),
	rotation(_rotation),
	scale(_scale),
	dirtyPosition(true),
	dirtyRotation(true),
	dirtyScale(true),
	dirtyTransform(true)
{}

const glm::vec3& Transform::getPosition() const {
	return this->position;
}

void Transform::setPosition(const glm::vec3& pos) {
	this->position = pos;
	this->dirtyPosition = true;
	this->dirtyTransform = true;
}

const glm::vec3& Transform::getRotation() const {
	return this->rotation;
}

void Transform::setRotation(const glm::vec3& rot) {
	this->rotation = rot;
	this->dirtyRotation = true;
	this->dirtyTransform = true;
}

const glm::vec3& Transform::getScale() const {
	return this->scale;
}

void Transform::setScale(const glm::vec3& _scale) {
	this->scale = _scale;
	this->dirtyScale = true;
	this->dirtyTransform = true;
}

const glm::mat4& Transform::getPositionMatrix() {
	if (this->dirtyPosition) {
		this->updatePositionMatrix();
		this->dirtyPosition = false;
	}
	return this->matPosition;
}

const glm::mat4& Transform::getRotationMatrix() {
	if (this->dirtyRotation) {
		this->updateRotationMatrix();
		this->dirtyRotation = false;
	}
	return this->matRotation;
}

const glm::mat4& Transform::getScaleMatrix() {
	if (this->dirtyScale) {
		this->updateScaleMatrix();
		this->dirtyScale = false;
	}
	return this->matScale;
}

const glm::mat4& Transform::getTransformMatrix() {
	if (this->dirtyTransform) {
		this->updateTransformMatrix();
		this->dirtyTransform = false;
	}
	return this->matTransform;
}

void Transform::updateTransformMatrix() {
	this->matTransform = this->getPositionMatrix() * this->getRotationMatrix() * this->getScaleMatrix();
}

void Transform::updateRotationMatrix() {
	// Create quaternions for each rotation axis
	const glm::quat pitch = glm::angleAxis(glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat yaw = glm::angleAxis(glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::quat roll = glm::angleAxis(glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Combine rotations by multiplying the quaternions
	const glm::quat orientation = roll * yaw * pitch;
	// Convert quaternion to rotation matrix
	this->matRotation = glm::mat4_cast(orientation);
}

void Transform::updatePositionMatrix() {
	this->matPosition = glm::translate(glm::mat4(1.0f), this->position);
}

void Transform::updateScaleMatrix() {
	this->matScale = glm::scale(glm::mat4(1.0f), this->scale);
}
