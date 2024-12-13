#pragma once

#include <glm/glm.hpp>
#include <string>

struct GLFWwindow;
class SceneNode;

class GUI {
private:
	static void drawMaterialProperties(const std::string& name, float& vector);
	static void drawMaterialProperties(const std::string& name, glm::vec2& vector);
	static void drawMaterialProperties(const std::string& name, glm::vec3& vector);
	static void drawMaterialProperties(const std::string& name, glm::vec4& vector);
	static void drawMaterialProperties(const std::string& name, int32_t& vector);
	static void drawMaterialProperties(const std::string& name, glm::ivec2& vector);
	static void drawMaterialProperties(const std::string& name, glm::ivec3& vector);
	static void drawMaterialProperties(const std::string& name, glm::ivec4& vector);
	static void drawMaterialProperties(const std::string& name, uint32_t& vector);
	static void drawMaterialProperties(const std::string& name, glm::uvec2& vector);
	static void drawMaterialProperties(const std::string& name, glm::uvec3& vector);
	static void drawMaterialProperties(const std::string& name, glm::uvec4& vector);

	static glm::uvec2 screenSize;

	void createNodeInputs(SceneNode*& node) const;
	void drawInspectorNode(SceneNode*& node) const;
public:
	GUI(GLFWwindow* window);
	~GUI();

	bool clickedOnUi() const;
	void newFrame(const glm::uvec2& dimensions) const;
	void drawSelection(SceneNode*& selectedObject) const;
	void drawInspector(SceneNode* root) const;
	void drawResources() const;
	void drawControls() const;
	void drawLightsEditor() const;
	void endRendering() const;
};
