#include "GUI.hpp"

#include "Material.hpp"
#include "MaterialLoader.hpp"
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
	const ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse || io.WantCaptureKeyboard;
}

void GUI::newFrame() const {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::drawSelection(MeshInstance* selectedObject) const {
	if (!selectedObject) {
		return;
	}
	ImGui::Begin("Selected Item", nullptr);
	// TODO: make object changable using dropdown menus
	ImGui::End();
}

void GUI::drawResources() const {
	ImGui::Begin("Resources", nullptr);
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
