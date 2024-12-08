#include "GUI.hpp"

#include "BoundingBox.hpp"
#include "MeshInstanceNode.hpp"
#include "LightSystem.hpp"
#include "Material.hpp"
#include "MaterialLoader.hpp"
#include "Shader.hpp"
#include "ShaderLoader.hpp"
#include "TextureLoader.hpp"
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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

void GUI::newFrame() const {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
/*
* TODO: FIX
void GUI::drawSelection(MeshInstanceNode* selectedObject) const {
	if (!selectedObject) {
		return;
	}
	ImGui::Begin("Selected Item", nullptr);
	ImGui::Text("Current selection: %s", selectedObject->name);
	uint32_t i = 0;
	for (auto [meshPtr, materialPtr, modelMatrix, bbox] : selectedObject->getDrawables()) {
		if (ImGui::TreeNode(("Mesh " + std::to_string(i++)).c_str())) {
			if (ImGui::BeginCombo("Shader", materialPtr->getShader()->name.c_str())) {
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
						// selectedObject->setMaterial(MaterialLoader::load(materialName));
					}
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
*/

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
	ImGui::Begin("Resources", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	if (ImGui::CollapsingHeader("Materials")) {
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
	}
	if (ImGui::CollapsingHeader("Shaders")) {
		for (const std::string& shaderName : ShaderLoader::getAllFileNames()) {
			ImGui::Separator();
			if (ShaderLoader::isLoaded(shaderName)) {
				ImGui::Text(shaderName.c_str());
			} else {
				if (ImGui::TreeNode(shaderName.c_str())) {
					if (ImGui::Button("Load Cubemap")) {
						ShaderLoader::load(shaderName);
					}
					ImGui::TreePop();
				}
			}
		}
	}
	if (ImGui::CollapsingHeader("Textures")) {
		for (const std::string& textureName : TextureLoader::getAllTextureNames()) {
			ImGui::Separator();
			if (TextureLoader::isLoaded(textureName)) {
				ImGui::Text(textureName.c_str());
			} else {
				if (ImGui::TreeNode(textureName.c_str())) {
					if (ImGui::Button("Load Cubemap")) {
						TextureLoader::load(textureName);
					}
					ImGui::TreePop();
				}
			}
		}
	}
	if (ImGui::CollapsingHeader("Cubemaps")) {
		for (const std::string& cubemapName : TextureLoader::getAllCubemapNames()) {
			ImGui::Separator();
			if (TextureLoader::isLoaded(cubemapName)) {
				ImGui::Text(cubemapName.c_str());
			} else {
				if (ImGui::TreeNode(cubemapName.c_str())) {
					if (ImGui::Button("Load Cubemap")) {
						TextureLoader::loadCubemap(cubemapName);
					}
					ImGui::TreePop();
				}
			}
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
