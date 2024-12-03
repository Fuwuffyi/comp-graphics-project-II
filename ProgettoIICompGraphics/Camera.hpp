#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 cameraMatrix;

	bool dirtyView;
	bool dirtyProjection;
	bool dirtyCamera;

	glm::vec3 position;
	glm::vec3 vectorUp;
	glm::vec3 viewDir;

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
	 * \param pos The camera's position.
	 * \param vUp The camera's up vector.
	 * \param dir The camera's view direction.
	 * \param _fov The camera's field of view.
	 * \param _aspectRatio The camera's width/height ratio.
	 * \param _near The near clip plane value.
	 * \param _far The far clip plane value.
	 */
	Camera(const glm::vec3& pos, const glm::vec3& vUp, const glm::vec3& dir, const float _fov, const float _aspectRatio, const float _near, const float _far);

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
};