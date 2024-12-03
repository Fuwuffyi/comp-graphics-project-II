#pragma once

#include "Transform.hpp"

class Camera {
private:
	glm::mat4 projectionMatrix;
	glm::mat4 cameraMatrix;

	bool dirtyProjection;
	bool dirtyCamera;

	float fov;
	float aspectRatio;
	float near;
	float far;

private:
	/*
	* Updates the camera's projection matrix.
	*/
	void updateProjectionMatrix();

	/*
	* Updates the camera's complete matrix.
	*/
	void updateCameraMatrix();

public:
	/**
	 * Creates a new camera at the given position.
	 *
	 * \param _transform The camera's transform.
	 */
	Camera(const float _fov, const float _aspectRatio, const float _near, const float _far);

	/**
	 * Getter for the camera's projection matrix.
	 *
	 * \return The camera's projection matrix.
	 */
	const glm::mat4& getProjectionMatrix();

	/**
	 * Getter for the camera's combined matrix.
	 *
	 * \return The camera's combined matrix.
	 */
	const glm::mat4& getCameraMatrix();
};