#include "GUI.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>

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
	// TODO: fill up with stuff
	if (ImGui::CollapsingHeader("Materials")) {
		for (int32_t i = 0; i < 10; ++i) {
			if (ImGui::TreeNode("Material")) {
				ImGui::Text("Dummy data");
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("Shaders")) {
		for (int32_t i = 0; i < 10; ++i) {
			if (ImGui::TreeNode("Shader")) {
				ImGui::Text("Dummy data");
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("Textures")) {
		for (int32_t i = 0; i < 10; ++i) {
			if (ImGui::TreeNode("Texture")) {
				ImGui::Text("Dummy data");
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void GUI::endRendering() const {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}