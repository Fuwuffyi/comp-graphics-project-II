#include "MeshLoader.hpp"

#include "MaterialLoader.hpp"
#include "Mesh.hpp"
#include "MeshInstanceNode.hpp"
#include "SceneNode.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <glad/glad.h>
#include <stdexcept>

namespace MeshLoader {
    static std::shared_ptr<MeshInstanceNode> processMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<SceneNode>& parent = nullptr);
    static std::shared_ptr<SceneNode> processNode(aiNode* node, const aiScene* scene, const std::shared_ptr<SceneNode>&parent = nullptr);

    static constexpr glm::mat4 mat4ToGlm(const aiMatrix4x4& aiMat);
    static std::string getNodeName(const aiString& str, const std::string& parentStr = "");
    
    static std::string currentFile = "";
    static uint32_t currentNodeIndex = 0;
}

constexpr glm::mat4 MeshLoader::mat4ToGlm(const aiMatrix4x4& aiMat) {
    return glm::mat4(
        aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
        aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
        aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
        aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    );
}

std::string MeshLoader::getNodeName(const aiString& str, const std::string& parentStr) {
    std::string nodeName = std::string(str.C_Str());
    if (nodeName.empty()) {
        if (parentStr.empty()) {
            nodeName = std::filesystem::path(currentFile).stem().string() + "_root";
        } else {
            nodeName = parentStr + "_child_" + std::to_string(currentNodeIndex++);
        }
    }
    return nodeName;
}

std::shared_ptr<MeshInstanceNode> MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<SceneNode>& parent) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.uv = glm::vec2(0.0f);
        }
        // process vertex positions, normals and texture coordinates
        vertices.emplace_back(vertex);
    }
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    std::shared_ptr<Mesh> loadedMesh = std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
    return std::make_shared<MeshInstanceNode>(
        getNodeName(mesh->mName, parent ? parent->name : ""), 
        loadedMesh, 
        MaterialLoader::load("phong"), 
        Transform(),
        parent);
}

std::shared_ptr<SceneNode> MeshLoader::processNode(aiNode* node, const aiScene* scene, const std::shared_ptr<SceneNode>& parent) {
    const std::shared_ptr<SceneNode> currentNode = std::make_shared<SceneNode>(
        getNodeName(node->mName, parent ? parent->name : ""), 
        Transform(mat4ToGlm(node->mTransformation)), 
        parent
    );
    // Process all the node's meshes
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        currentNode->addChild(processMesh(mesh, scene, currentNode));
    }
    // Process all the node's children
    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        currentNode->addChild(processNode(node->mChildren[i], scene, currentNode));
    }
    return currentNode;
}

std::shared_ptr<SceneNode> MeshLoader::loadMesh(const std::string& fileName, const Transform& rootTransform) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_OptimizeMeshes | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Failed to open mesh file: " + fileName);
	}
    currentFile = fileName;
    currentNodeIndex = 0;
    const std::shared_ptr<SceneNode> rootNode = processNode(scene->mRootNode, scene);
    rootNode->setPosition(rootTransform.getPosition());
    rootNode->setRotation(rootTransform.getRotation());
    rootNode->setScale(rootTransform.getScale());
    importer.FreeScene();
	return rootNode;
}