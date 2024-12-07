#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Transform& _transform, const glm::vec3& vUp, const float _fov, const float _aspectRatio, const float _near, const float _far)
	:
	viewMatrix(1.0f),
	projectionMatrix(1.0f),
	cameraMatrix(1.0f),
	dirtyView(true),
	dirtyProjection(true),
	dirtyCamera(true),
	transform(_transform.getPosition(), _transform.getRotation()), // Make sure scale is default
	vectorUp(vUp),
	fov(_fov),
	aspectRatio(_aspectRatio),
	near(_near),
	far(_far)
{}

glm::vec3 Camera::getViewDirection() {
	const glm::mat4 rotationMatrix = this->transform.getRotationMatrix();
	return glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
}

const glm::vec3& Camera::getUpVector() {
	return this->vectorUp;
}

glm::vec3 Camera::getRightVector() {
	const glm::vec3 viewDir = this->getViewDirection();
	return glm::normalize(glm::cross(viewDir, this->vectorUp));
}

void Camera::updateViewMatrix() {
	const glm::vec3 cameraPos = this->transform.getPosition();
	const glm::vec3 viewDir = this->getViewDirection();
	this->viewMatrix = glm::lookAt(cameraPos, cameraPos + viewDir, this->vectorUp);
}

void Camera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(glm::radians(this->fov), this->aspectRatio, this->near, this->far);
}

void Camera::updateCameraMatrix() {
	this->cameraMatrix = this->getProjectionMatrix() * this->getViewMatrix();
}

float Camera::getFOV() const {
	return this->fov;
}

void Camera::setFOV(const float newFov) {
	this->dirtyProjection = true;
	this->dirtyCamera = true;
	this->fov = newFov;
}

void Camera::setAspectRatio(const float newRatio) {
	if (newRatio != this->aspectRatio) {
		this->dirtyProjection = true;
		this->dirtyCamera = true;
		this->aspectRatio = newRatio;
	}
}

const glm::mat4& Camera::getViewMatrix() {
	if (this->dirtyView) {
		this->updateViewMatrix();
		this->dirtyView = false;
	}
	return this->viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() {
	if (this->dirtyProjection) {
		this->updateProjectionMatrix();
		this->dirtyProjection = false;
	}
	return this->projectionMatrix;
}

const glm::mat4& Camera::getCameraMatrix() {
	if (this->dirtyCamera) {
		this->updateCameraMatrix();
		this->dirtyCamera = false;
	}
	return this->cameraMatrix;
}

const Transform& Camera::getTransform() const {
	return this->transform;
}

Transform& Camera::getMutableTransform() {
	this->dirtyView = true;
	this->dirtyCamera = true;
	return this->transform;
}
