#include "MainScene.hpp"

#include "MeshLoader.hpp"

#include <random>

namespace MainScene {
	static std::shared_ptr<SceneNode> getSea();
	static std::shared_ptr<SceneNode> getGrass();
	static std::shared_ptr<SceneNode> getCity();

	static std::random_device randDevice;
	static std::mt19937 randEngine(randDevice());
}

std::shared_ptr<SceneNode> MainScene::getSea() {
	std::shared_ptr<SceneNode> sea = std::make_shared<SceneNode>("Sea", Transform());
	// Create sea
	std::shared_ptr<SceneNode> seaNode = std::make_shared<MeshInstanceNode>("Sea", Primitives::generatePlane(500), MaterialLoader::load("water"), Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(150.0f)), sea);
	sea->addChild(seaNode);
	std::shared_ptr<SceneNode> seaFloor = std::make_shared<MeshInstanceNode>("Seafloor", Primitives::generatePlane(1), MaterialLoader::load("seafloor"), Transform(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f), glm::vec3(200.0f)), sea);
	sea->addChild(seaFloor);
	// Add some doughnuts
	std::uniform_real_distribution<float> distX(-30.0f, 30.0f);
	std::uniform_real_distribution<float> distZ(6.0f, 15.0f);
	for (uint32_t i = 0; i < 12; ++i) {
		std::shared_ptr<SceneNode> doughnut = std::make_shared<MeshInstanceNode>("Doughnut", Primitives::generateThorus(1.0f, 0.5f, 25, 25), MaterialLoader::load("blinn_phong"), Transform(glm::vec3(distX(randEngine), 0.0f, distZ(randEngine)), glm::vec3(0.0f), glm::vec3(0.3f)), sea);
		doughnut->name = doughnut->name + std::to_string(i);
		doughnut->setParent(sea);
		sea->addChild(doughnut);
	}
	// Return the sea
	return sea;
}

std::shared_ptr<SceneNode> MainScene::getGrass() {
	// Create base grass plane
	std::shared_ptr<SceneNode> grass = std::make_shared<MeshInstanceNode>("GrassPlane", Primitives::generatePlane(1, glm::vec2(35.0f, 11.0f)), MaterialLoader::load("grass"), Transform(glm::vec3(0.0f, 1.99f, -12.0f), glm::vec3(0.0f), glm::vec3(70.0f, 1.0, 22.0f)));
	const glm::vec3 baseScale(1.0f / glm::vec3(70.0f, 1.0f, 22.0f));
	// Add trees
	std::shared_ptr<SceneNode> trees = std::make_shared<SceneNode>("Trees", Transform(), grass);
	grass->addChild(trees);
	std::uniform_real_distribution<float> distX(0.04f, 0.47f);
	std::uniform_real_distribution<float> distZ(-0.43f, 0.23f);
	std::uniform_real_distribution<float> distScl(0.8f, 1.2f);
	std::unordered_map<uint32_t, std::shared_ptr<Material>> treeOverrides = {
		{ 0, MaterialLoader::load("leaves")}
	};
	// Right trees
	for (uint32_t i = 0; i < 100; ++i) {
		std::shared_ptr<SceneNode> tree = MeshLoader::loadMesh("assets/meshes/PineTree/scene.gltf", Transform(glm::vec3(-distX(randEngine), 0.0f, distZ(randEngine)), glm::vec3(-90.0f, 0.0f, 0.0f), distScl(randEngine) * baseScale), treeOverrides);
		tree->name = tree->name + "R" + std::to_string(i);
		tree->setParent(trees);
		trees->addChild(tree);
	}
	// Left trees
	for (uint32_t i = 0; i < 100; ++i) {
		std::shared_ptr<SceneNode> tree = MeshLoader::loadMesh("assets/meshes/PineTree/scene.gltf", Transform(glm::vec3(distX(randEngine), 0.0f, distZ(randEngine)), glm::vec3(-90.0f, 0.0f, 0.0f), distScl(randEngine) * baseScale), treeOverrides);
		tree->name = tree->name + "L" + std::to_string(i);
		tree->setParent(trees);
		trees->addChild(tree);
	}
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
	std::shared_ptr<SceneNode> lights = std::make_shared<SceneNode>("Lights", Transform(), city);
	city->addChild(lights);
	// Right lights
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(6.5f + 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.1f)));
		lightMesh->name = lightMesh->name + "R" + std::to_string(i);
		lightMesh->setParent(lights);
		lights->addChild(lightMesh);
	}
	// Left lights
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(-6.5f - 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.1f)));
		lightMesh->name = lightMesh->name + "L" + std::to_string(i);
		lightMesh->setParent(lights);
		lights->addChild(lightMesh);
	}
	// Get grass section
	std::shared_ptr<SceneNode> grass = getGrass();
	grass->setParent(city);
	city->addChild(grass);
	// Add houses
	std::shared_ptr<SceneNode> houses = std::make_shared<SceneNode>("Houses", Transform(), city);
	city->addChild(houses);
	// Add Houses
	for (uint32_t i = 0; i < 6; ++i) {
		std::shared_ptr<SceneNode> house = MeshLoader::loadMesh("assets/meshes/LisboaHouse/scene.gltf", Transform(glm::vec3(6.15f + 5.04 * i, 3.53f, -4.563f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(1.15f)));
		house->name = house->name + "R" + std::to_string(i);
		house->setParent(houses);
		houses->addChild(house);
	}
	for (uint32_t i = 0; i < 6; ++i) {
		std::shared_ptr<SceneNode> house = MeshLoader::loadMesh("assets/meshes/LisboaHouse/scene.gltf", Transform(glm::vec3(-7.7f - 5.04 * i, 3.53f, -4.563f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(1.15f)));
		house->name = house->name + "L" + std::to_string(i);
		house->setParent(houses);
		houses->addChild(house);
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