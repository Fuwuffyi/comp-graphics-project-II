#pragma once

#include "Transform.hpp"

class Camera {
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 cameraMatrix;

	bool dirtyView;
	bool dirtyProjection;
	bool dirtyCamera;

	Transform transform;
	glm::vec3 vectorUp;

	float fov;
	float aspectRatio;
	float near;
	float far;

private:
	/*
	* Updates the camera's view matrix.
	*/
	void updateViewMatrix();

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
	 * \param _transform The camera's transform properties.
	 * \param vUp The camera's up vector.
	 * \param _fov The camera's field of view.
	 * \param _aspectRatio The camera's width/height ratio.
	 * \param _near The near clip plane value.
	 * \param _far The far clip plane value.
	 */
	Camera(const Transform& _transform, const glm::vec3& vUp, const float _fov, const float _aspectRatio, const float _near, const float _far);

	/**
	 * Getter for the camera's view matrix.
	 *
	 * \return The camera's view matrix.
	 */
	const glm::mat4& getViewMatrix();

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

	/**
	 * Getter for the camera's trasform.
	 *
	 * \return The camera's transform reference.
	 */
	const Transform& getTransform() const;
};