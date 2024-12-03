#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& pos, const glm::vec3& vUp, const glm::vec3& dir, const float _fov, const float _aspectRatio, const float _near, const float _far)
	:
	viewMatrix(1.0f),
	projectionMatrix(1.0f),
	cameraMatrix(1.0f),
	dirtyView(true),
	dirtyProjection(true),
	dirtyCamera(true),
	position(pos),
	vectorUp(vUp),
	viewDir(dir),
	fov(_fov),
	aspectRatio(_aspectRatio),
	near(_near),
	far(_far)
{}

void Camera::updateViewMatrix() {
	this->viewMatrix = glm::lookAt(this->position, this->position + glm::normalize(this->viewDir), this->vectorUp);
}

void Camera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(glm::radians(this->fov), this->aspectRatio, this->near, this->far);
}

void Camera::updateCameraMatrix() {
	this->cameraMatrix = this->getProjectionMatrix() * this->getViewMatrix();
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