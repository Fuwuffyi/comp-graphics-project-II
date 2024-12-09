#include "MainScene.hpp"

namespace MainScene {
	static std::shared_ptr<SceneNode> getSea();
}

std::shared_ptr<SceneNode> MainScene::getSea() {
	std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>("Sea", Transform());
	// Create sea
	std::shared_ptr<SceneNode> seaNode = std::make_shared<MeshInstanceNode>("Sea", Primitives::generatePlane(500), MaterialLoader::load("water"), Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(150.0f)), scene);
	scene->addChild(seaNode);
	std::shared_ptr<SceneNode> seaFloor = std::make_shared<MeshInstanceNode>("Seafloor", Primitives::generatePlane(1), MaterialLoader::load("seafloor"), Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(200.0f)), scene);
	scene->addChild(seaFloor);
	// Return the sea
	return scene;
}

std::shared_ptr<SceneNode> MainScene::getScene() {
	// Create an empty scene
	std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>("Scene", Transform());
	// Add sea
	std::shared_ptr<SceneNode> sea = getSea();
	scene->addChild(sea);
	sea->setParent(scene);
	// Return the scene
	return scene;
}