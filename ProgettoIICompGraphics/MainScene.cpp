#include "MainScene.hpp"

#include "MeshLoader.hpp"

namespace MainScene {
	static std::shared_ptr<SceneNode> getSea();
	static std::shared_ptr<SceneNode> getGrass();
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

std::shared_ptr<SceneNode> MainScene::getGrass() {
	// Create base grass plane
	std::shared_ptr<SceneNode> grass = std::make_shared<MeshInstanceNode>("GrassPlane", Primitives::generatePlane(1), MaterialLoader::load("grass"), Transform(glm::vec3(0.0f, 1.99f, -12.0f), glm::vec3(0.0f), glm::vec3(70.0f, 1.0, 22.0f)));
	const glm::vec3 baseScale(1.0f / glm::vec3(60.0f, 1.0, 30.0f));
	// Return grass and foliage
	return grass;
}

std::shared_ptr<SceneNode> MainScene::getCity() {
	std::shared_ptr<SceneNode> city = std::make_shared<SceneNode>("City", Transform());
	// Add walkway lights
	std::shared_ptr<SceneNode> walkway = MeshLoader::loadMesh("assets/meshes/walkways.obj", Transform());
	walkway->setParent(city);
	city->addChild(walkway);
	// Add cool fountain
	std::shared_ptr<SceneNode> fountain = MeshLoader::loadMesh("assets/meshes/Fountain/scene.gltf", Transform(glm::vec3(-0.15f, 3.67f, 0.8f), glm::vec3(-180.0f, -90.0f, 0.0f), glm::vec3(0.6)));
	fountain->setParent(walkway);
	walkway->addChild(fountain);
	// Add lights
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(6.5f + 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.1f)));
		lightMesh->name = lightMesh->name + "R" + std::to_string(i);
		lightMesh->setParent(city);
		city->addChild(lightMesh);
	}
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(-6.5f - 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.1f)));
		lightMesh->name = lightMesh->name + "L" + std::to_string(i);
		lightMesh->setParent(city);
		city->addChild(lightMesh);
	}
	// Get grass section
	std::shared_ptr<SceneNode> grass = getGrass();
	grass->setParent(city);
	city->addChild(grass);
	// Add Houses
	for (uint32_t i = 0; i < 6; ++i) {
		std::shared_ptr<SceneNode> house = MeshLoader::loadMesh("assets/meshes/LisboaHouse/scene.gltf", Transform(glm::vec3(6.15f + 5.04 * i, 3.53f, -4.563f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(1.15f)));
		house->name = house->name + "R" + std::to_string(i);
		house->setParent(city);
		city->addChild(house);
	}
	for (uint32_t i = 0; i < 6; ++i) {
		std::shared_ptr<SceneNode> house = MeshLoader::loadMesh("assets/meshes/LisboaHouse/scene.gltf", Transform(glm::vec3(-7.7f - 5.04 * i, 3.53f, -4.563f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(1.15f)));
		house->name = house->name + "L" + std::to_string(i);
		house->setParent(city);
		city->addChild(house);
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