#include "GUI.hpp"

#include "BoundingBox.hpp"
#include "LightSystem.hpp"
#include "Material.hpp"
#include "MaterialLoader.hpp"
#include "MeshInstanceNode.hpp"
#include "SceneNode.hpp"
#include "Shader.hpp"
#include "ShaderLoader.hpp"
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

glm::uvec2 GUI::screenSize = glm::uvec2(0);

GUI::GUI(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

GUI::~GUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool GUI::clickedOnUi() const {
	return ImGui::GetIO().WantCaptureMouse;
}

void GUI::newFrame(const glm::uvec2& dimensions) const {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	GUI::screenSize = dimensions;
}

void GUI::createNodeInputs(SceneNode*& node) const {
	glm::vec3 input = node->getLocalTransform().getPosition();
	if (ImGui::TreeNode((node->name + " position").c_str())) {
		if (ImGui::InputFloat3("Position", &input.x)) {
			node->setPosition(input);
		}
		input = node->getLocalTransform().getRotation();
		if (ImGui::InputFloat3("Rotation", &input.x)) {
			node->setRotation(input);
		}
		input = node->getLocalTransform().getScale();
		if (ImGui::InputFloat3("Scale", &input.x)) {
			node->setScale(input);
		}
		ImGui::TreePop();
	}
	if (auto meshInstanceChild = dynamic_cast<MeshInstanceNode*>(node)) {
		Material* materialPtr = meshInstanceChild->getMaterial().get();
		Shader* shaderPtr = materialPtr->getShader();
		if (ImGui::BeginCombo("Shader", shaderPtr->name.c_str())) {
			for (const std::string& shaderName : ShaderLoader::getAllFileNames()) {
				if (ImGui::Selectable(shaderName.c_str())) {
					materialPtr->setShader(ShaderLoader::load(shaderName));
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::BeginCombo("Material", materialPtr->name.c_str())) {
			for (const std::string& materialName : MaterialLoader::getAllFileNames()) {
				if (ImGui::Selectable(materialName.c_str())) {
					meshInstanceChild->setMaterial(MaterialLoader::load(materialName));
				}
			}
			ImGui::EndCombo();
		}
	}
}

void GUI::drawInspectorNode(SceneNode*& node) const {
	if (ImGui::TreeNode(node->name.c_str())) {
		createNodeInputs(node);
		const auto children = node->getChildren();
		if (children.size() > 0) {
			if (ImGui::TreeNode((node->name + " children").c_str())) {
				for (auto child : children) {
					auto ptr = child.get();
					drawInspectorNode(ptr);
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void GUI::drawInspector(SceneNode* root) const {
	ImGui::SetNextWindowPos(ImVec2(screenSize.x - 500, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(500, screenSize.y), ImGuiCond_Always);
	ImGui::Begin("Scene Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize);
	this->drawInspectorNode(root);
	ImGui::End();
}

void GUI::drawSelection(SceneNode*& selectedObject) const {
	if (selectedObject == nullptr) {
		return;
	}
	ImGui::SetNextWindowPos(ImVec2(0, screenSize.y - 200), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x, 200), ImGuiCond_Always);
	ImGui::Begin("Selected Item", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::Text("Current selection: %s", selectedObject->name.c_str());
	if (selectedObject->getParent()) {
		if (ImGui::Button("Select parent")) {
			selectedObject = selectedObject->getParent().get();
		}
	}
	createNodeInputs(selectedObject);
	ImGui::End();
}

void GUI::drawLightsEditor() const {
	static const char* items[4] = { "NONE", "DIRECTIONAL", "POINT", "SPOT" };
	// Create the window
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("Lights editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	// Read the lights
	LightSystem::Lights& lights = LightSystem::getAllLights();
	// Show lights in imgui
	for (size_t i = 0; i < LightSystem::MAX_LIGHTS; ++i) {
		if (ImGui::TreeNode(("Light " + std::to_string(i)).c_str())) {
			LightSystem::Light& light = lights.lights[i];
			if (ImGui::Combo("Type", reinterpret_cast<int32_t*>(&light.type), items, 4)) {
				LightSystem::setLight(i, light);
			}
			if (ImGui::ColorEdit3("Ambient", &light.ambient.x)) {
				LightSystem::setLight(i, light);
			}
			if (ImGui::ColorEdit3("Diffuse", &light.diffuse.x)) {
				LightSystem::setLight(i, light);
			}
			if (ImGui::ColorEdit3("Specular", &light.specular.x)) {
				LightSystem::setLight(i, light);
			}
			switch (light.type) {
				case LightSystem::LIGHT_TYPE::NONE:
					break;
				case LightSystem::LIGHT_TYPE::DIRECTIONAL:
					if (ImGui::SliderFloat3("Direction", &light.direction.x, -1.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					break;
				case LightSystem::LIGHT_TYPE::POINT:
					if (ImGui::SliderFloat3("Position", &light.position.x, -10.0f, 10.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Range", &light.range, 0.0f, 100.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Linear", &light.linear, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Constant", &light.constant, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Quadratic", &light.quadratic, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					break;
				case LightSystem::LIGHT_TYPE::SPOT:
					if (ImGui::SliderFloat3("Position", &light.position.x, -10.0f, 10.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat3("Direction", &light.direction.x, -1.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Range", &light.range, 0.0f, 100.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Linear", &light.linear, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Constant", &light.constant, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Quadratic", &light.quadratic, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Cutoff", &light.cutOff, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					if (ImGui::SliderFloat("Outer Cutoff", &light.outerCutOff, 0.0f, 1.0f)) {
						LightSystem::setLight(i, light);
					}
					break;
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void GUI::drawResources() const {
	ImGui::Begin("Materials", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	for (const std::string& materialName : MaterialLoader::getAllFileNames()) {
		ImGui::Separator();
		if (ImGui::TreeNode((materialName + ".material").c_str())) {
			if (MaterialLoader::isLoaded(materialName)) {
				for (auto& [uniform, value] : MaterialLoader::load(materialName).get()->getMutableProperties()) {
					auto visitor = [&](auto& val) {
						using T = std::decay_t<decltype(val)>;
						GUI::drawMaterialProperties(uniform, val);
					};
					std::visit(visitor, value);
				}
			} else {
				if (ImGui::Button("Load Material")) {
					MaterialLoader::load(materialName);
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void GUI::endRendering() const {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::drawMaterialProperties(const std::string& name, float& vector) {
	ImGui::InputFloat(name.c_str(), &vector);
}

void GUI::drawMaterialProperties(const std::string& name, glm::vec2& vector) {
	ImGui::InputFloat2(name.c_str(), &vector.x);
}

void GUI::drawMaterialProperties(const std::string& name, glm::vec3& vector) {
	ImGui::InputFloat3(name.c_str(), &vector.x);
}

void GUI::drawMaterialProperties(const std::string& name, glm::vec4& vector) {
	ImGui::InputFloat4(name.c_str(), &vector.x);
}

void GUI::drawMaterialProperties(const std::string& name, int32_t& vector) {
	ImGui::InputInt(name.c_str(), &vector);
}

void GUI::drawMaterialProperties(const std::string& name, glm::ivec2& vector) {
	ImGui::InputInt2(name.c_str(), &vector.x);
}

void GUI::drawMaterialProperties(const std::string& name, glm::ivec3& vector) {
	ImGui::InputInt3(name.c_str(), &vector.x);
}

void GUI::drawMaterialProperties(const std::string& name, glm::ivec4& vector) {
	ImGui::InputInt4(name.c_str(), &vector.x);
}

void GUI::drawMaterialProperties(const std::string& name, uint32_t& vector) {
	ImGui::InputInt(name.c_str(), reinterpret_cast<int32_t *>(& vector));
}

void GUI::drawMaterialProperties(const std::string& name, glm::uvec2& vector) {
	ImGui::InputInt2(name.c_str(), reinterpret_cast<int32_t*>(&vector.x));
}

void GUI::drawMaterialProperties(const std::string& name, glm::uvec3& vector) {
	ImGui::InputInt3(name.c_str(), reinterpret_cast<int32_t*>(&vector.x));
}

void GUI::drawMaterialProperties(const std::string& name, glm::uvec4& vector) {
	ImGui::InputInt4(name.c_str(), reinterpret_cast<int32_t*>(&vector.x));
}
