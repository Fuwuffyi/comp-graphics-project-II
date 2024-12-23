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
	std::vector<std::shared_ptr<Material>> doughnutMaterials = {
		MaterialLoader::load("doughnutA"),
		MaterialLoader::load("doughnutB"),
		MaterialLoader::load("doughnutC")
	};
	std::uniform_real_distribution<float> distX(-30.0f, 30.0f);
	std::uniform_real_distribution<float> distZ(-15.0f, -6.0f);
	std::uniform_int_distribution<uint32_t> matRand(0, 2);
	for (uint32_t i = 0; i < 12; ++i) {
		std::shared_ptr<SceneNode> doughnut = std::make_shared<MeshInstanceNode>("Doughnut", Primitives::generateThorus(1.0f, 0.5f, 15, 15), doughnutMaterials[matRand(randEngine)], Transform(glm::vec3(distX(randEngine), 0.0f, distZ(randEngine)), glm::vec3(0.0f), glm::vec3(0.3f)), sea);
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
	const std::unordered_map<uint32_t, std::shared_ptr<Material>> treeOverrides = {
		{ 0, MaterialLoader::load("leaves") }
	};
	// Right trees
	for (uint32_t i = 0; i < 120; ++i) {
		std::shared_ptr<SceneNode> tree = MeshLoader::loadMesh("assets/meshes/PineTree/scene.gltf", Transform(glm::vec3(-distX(randEngine), 0.0f, distZ(randEngine)), glm::vec3(-90.0f, 0.0f, 0.0f), distScl(randEngine) * baseScale), treeOverrides);
		tree->name = tree->name + "R" + std::to_string(i);
		tree->setParent(trees);
		trees->addChild(tree);
	}
	// Left trees
	for (uint32_t i = 0; i < 120; ++i) {
		std::shared_ptr<SceneNode> tree = MeshLoader::loadMesh("assets/meshes/PineTree/scene.gltf", Transform(glm::vec3(distX(randEngine), 0.0f, distZ(randEngine)), glm::vec3(-90.0f, 0.0f, 0.0f), distScl(randEngine) * baseScale), treeOverrides);
		tree->name = tree->name + "L" + std::to_string(i);
		tree->setParent(trees);
		trees->addChild(tree);
	}
	// Return grass and foliage
	return grass;
}

std::shared_ptr<SceneNode> MainScene::getCity() {
	std::shared_ptr<SceneNode> city = std::make_shared<SceneNode>("City", Transform(glm::vec3(0.0f), glm::vec3(0.0f, 180.0f, 0.0f)));
	// Add walkway lights
	const std::unordered_map<uint32_t, std::shared_ptr<Material>> walkwayOverrides = {
		{ 0, MaterialLoader::load("bricks") }
	};
	std::shared_ptr<SceneNode> walkway = MeshLoader::loadMesh("assets/meshes/walkways.obj", Transform(), walkwayOverrides);
	walkway->setParent(city);
	city->addChild(walkway);
	// Add cool fountain
	const std::unordered_map<uint32_t, std::shared_ptr<Material>> statueOverrides = {
		{ 0, MaterialLoader::load("marble") }
	};
	std::shared_ptr<SceneNode> centralStatue = MeshLoader::loadMesh("assets/meshes/DragonStatue/dragon.glb", Transform(glm::vec3(0.0f, 4.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.025f)), statueOverrides);
	centralStatue->setParent(walkway);
	walkway->addChild(centralStatue);
	// Add lights
	std::shared_ptr<SceneNode> lights = std::make_shared<SceneNode>("Lights", Transform(), walkway);
	walkway->addChild(lights);
	const std::unordered_map<uint32_t, std::shared_ptr<Material>> lightsOverrides = {
		{ 0, MaterialLoader::load("lampPost") },
		{ 3, MaterialLoader::load("lightGlass") }
	};
	// Right lights
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(6.5f + 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.1f)), lightsOverrides);
		lightMesh->name = lightMesh->name + "R" + std::to_string(i);
		lightMesh->setParent(lights);
		lights->addChild(lightMesh);
	}
	// Left lights
	for (uint32_t i = 0; i < 8; ++i) {
		std::shared_ptr<SceneNode> lightMesh = MeshLoader::loadMesh("assets/meshes/rv_lamp_post_4.obj", Transform(glm::vec3(-6.5f - 3.8f * i, 2.15f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.1f)), lightsOverrides);
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