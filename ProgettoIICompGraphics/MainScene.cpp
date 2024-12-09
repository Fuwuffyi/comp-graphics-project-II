#include "MainScene.hpp"

#include "MeshLoader.hpp"

namespace MainScene {
	static std::shared_ptr<SceneNode> getSea();
	static std::shared_ptr<SceneNode> getCity();
}

std::shared_ptr<SceneNode> MainScene::getSea() {
	std::shared_ptr<SceneNode> sea = std::make_shared<SceneNode>("Sea", Transform());
	// Create sea
	std::shared_ptr<SceneNode> seaNode = std::make_shared<MeshInstanceNode>("Sea", Primitives::generatePlane(500), MaterialLoader::load("water"), Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(150.0f)), sea);
	sea->addChild(seaNode);
	std::shared_ptr<SceneNode> seaFloor = std::make_shared<MeshInstanceNode>("Seafloor", Primitives::generatePlane(1), MaterialLoader::load("seafloor"), Transform(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(200.0f)), sea);
	sea->addChild(seaFloor);
	// Return the sea
	return sea;
}

std::shared_ptr<SceneNode> MainScene::getCity() {
	std::shared_ptr<SceneNode> city = std::make_shared<SceneNode>("City", Transform());
	// Add walkway lights
	std::shared_ptr<SceneNode> walkway = MeshLoader::loadMesh("assets/meshes/walkways.obj", Transform());
	walkway->setParent(city);
	city->addChild(walkway);
	// Add lights
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(6.5f + 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.15f)));
		lightMesh->name = lightMesh->name + "R" + std::to_string(i);
		lightMesh->setParent(city);
		city->addChild(lightMesh);
	}
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(-6.5f - 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.15f)));
		lightMesh->name = lightMesh->name + "L" + std::to_string(i);
		lightMesh->setParent(city);
		city->addChild(lightMesh);
	}
	// Return the city
	return city;
}

std::shared_ptr<SceneNode> MainScene::getScene() {
	// Create an empty scene
	std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>("Scene", Transform());
	// Add sea
	std::shared_ptr<SceneNode> sea = getSea();
	scene->addChild(sea);
	sea->setParent(scene);
	// Add city
	std::shared_ptr<SceneNode> city = getCity();
	scene->addChild(city);
	city->setParent(scene);
	// Return the scene
	return scene;
}