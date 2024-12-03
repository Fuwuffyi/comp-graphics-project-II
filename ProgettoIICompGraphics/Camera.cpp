#include "Camera.hpp"

Camera::Camera(const float _fov, const float _aspectRatio, const float _near, const float _far)
	:
	projectionMatrix(1.0f),
	cameraMatrix(1.0f),
	dirtyProjection(true),
	dirtyCamera(true),
	fov(_fov),
	aspectRatio(_aspectRatio),
	near(_near),
	far(_far)
{}

void Camera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(glm::radians(this->fov), this->aspectRatio, this->near, this->far);
}

void Camera::updateCameraMatrix() {
	this->cameraMatrix = glm::mat4(1.0f) * this->getProjectionMatrix();
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