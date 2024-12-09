#include "MeshInstanceNode.hpp"

#include "Mesh.hpp"

MeshInstanceNode::MeshInstanceNode(const std::string& _name, const std::shared_ptr<Mesh>& _mesh, const std::shared_ptr<Material>& _material, const Transform& _transform, const std::shared_ptr<SceneNode>& parent)
	:
	SceneNode(_name, _transform, parent),
	mesh(_mesh),
	material(_material),
	boundingBox(this->mesh->getBoundingBox())
{}

void MeshInstanceNode::updateWorldTransform() {
	SceneNode::updateWorldTransform();
	boundingBox = this->mesh->getBoundingBox().transform(this->localTransform.getTransformMatrix());
	if (this->parentNode) {
		boundingBox = boundingBox.transform(this->parentNode->getWorldTransform().getTransformMatrix());
	}
}

Mesh* MeshInstanceNode::getMesh() const {
	return this->mesh.get();
}

const std::shared_ptr<Material>& MeshInstanceNode::getMaterial() const {
	return this->material;
}

void MeshInstanceNode::setMaterial(const std::shared_ptr<Material>& _material) {
	this->material = _material;
}

const BoundingBox MeshInstanceNode::getBoundingBox() const {
	return this->boundingBox;
}
