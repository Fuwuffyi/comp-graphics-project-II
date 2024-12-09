#pragma once

#include "SceneNode.hpp"
#include "BoundingBox.hpp"

class Mesh;
class Material;

class MeshInstanceNode : public SceneNode {
private:
	Mesh* mesh;
	std::shared_ptr<Material> material;
	BoundingBox boundingBox;
protected:
	virtual void updateWorldTransform() override;
public:
	MeshInstanceNode(const std::string& _name, Mesh* _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform, const std::shared_ptr<SceneNode>& parent = nullptr);

	Mesh* getMesh() const;
	const std::shared_ptr<Material>& getMaterial() const;
	void setMaterial(const std::shared_ptr<Material>& _material);
	const BoundingBox getBoundingBox() const;
};
