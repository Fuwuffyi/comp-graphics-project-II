#pragma once

#include <glm/glm.hpp>

class Transform {
protected:
	glm::mat4 matPosition;
	glm::mat4 matScale;
	glm::mat4 matRotation;
	glm::mat4 matTransform;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	bool dirtyPosition;
	bool dirtyScale;
	bool dirtyRotation;
	bool dirtyTransform;
public:
	Transform(const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _rotation = glm::vec3(0.0f), const glm::vec3& _scale = glm::vec3(1.0f));
	Transform(const glm::mat4& transformMatrix);

	const glm::vec3& getPosition() const;
	void setPosition(const glm::vec3& pos);

	const glm::vec3& getRotation() const;
	void setRotation(const glm::vec3& rot);

	const glm::vec3& getScale() const;
	void setScale(const glm::vec3& _scale);

	const glm::mat4& getPositionMatrix() const;
	const glm::mat4& getRotationMatrix() const;
	const glm::mat4& getScaleMatrix() const;
	const glm::mat4& getTransformMatrix() const;

	Transform operator*(const Transform& other) const;

protected:
	void updatePositionMatrix();
	void updateRotationMatrix();
	void updateScaleMatrix();
	void updateTransformMatrix();
};
