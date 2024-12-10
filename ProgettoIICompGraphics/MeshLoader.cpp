#include "MeshLoader.hpp"

#include "MaterialLoader.hpp"
#include "Mesh.hpp"
#include "MeshInstanceNode.hpp"
#include "SceneNode.hpp"
#include "TextureLoader.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <glad/glad.h>
#include <stdexcept>

namespace MeshLoader {
    static std::unordered_map<std::string, std::pair<std::shared_ptr<Material>, std::shared_ptr<Mesh>>> loadedMeshes;

    static std::shared_ptr<MeshInstanceNode> processMesh(aiMesh* mesh, const aiScene* scene, const std::vector<std::shared_ptr<Material>>& materialOverrides = std::vector<std::shared_ptr<Material>>(), const std::shared_ptr<SceneNode>& parent = nullptr);
    static std::shared_ptr<SceneNode> processNode(aiNode* node, const aiScene* scene, const std::vector<std::shared_ptr<Material>>& materialOverrides = std::vector<std::shared_ptr<Material>>(), const std::shared_ptr<SceneNode>& parent = nullptr);

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
    const std::string nodeName = std::string(str.C_Str());
    if (!nodeName.empty()) {
        return nodeName;
    }
    if (parentStr.empty()) {
        return std::filesystem::path(currentFile).stem().string() + "_root";
    }
    return parentStr + "_child_" + std::to_string(currentNodeIndex++);
}

std::shared_ptr<MeshInstanceNode> MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene, const std::vector<std::shared_ptr<Material>>& materialOverrides, const std::shared_ptr<SceneNode>& parent) {
    if (loadedMeshes.find(getNodeName(mesh->mName, parent ? parent->name : "")) != loadedMeshes.end()) {
        return std::make_shared<MeshInstanceNode>(
            getNodeName(mesh->mName, parent ? parent->name : ""),
            loadedMeshes.at(getNodeName(mesh->mName, parent ? parent->name : "")).second,
            loadedMeshes.at(getNodeName(mesh->mName, parent ? parent->name : "")).first,
            Transform(),
            parent);
    }
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
    std::shared_ptr<Material> material = nullptr;
    if (mesh->mMaterialIndex < scene->mNumMaterials) {
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
        const std::string matName = std::string(aiMaterial->GetName().C_Str());
        if (mesh->mMaterialIndex < materialOverrides.size()) {
            material = materialOverrides[mesh->mMaterialIndex];
        } else {
            if (MaterialLoader::isLoaded(matName)) {
                material = MaterialLoader::load(matName);
            } else {
                std::unordered_map<std::string, Material::MaterialValueType> materialProperties;
                std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
                // Setup base color
                aiColor4D color;
                if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_BASE_COLOR, color)) {
                    color = aiColor4D(1.0f);
                }
                materialProperties.emplace("color", glm::vec4(color.r, color.g, color.b, color.a));
                // Setup ambient color
                aiColor4D ambient;
                if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient)) {
                    ambient = aiColor4D(1.0f);
                }
                materialProperties.emplace("ambient", glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a));
                // Setup diffuse color
                aiColor4D diffuse;
                if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
                    diffuse = aiColor4D(1.0f);
                }
                materialProperties.emplace("diffuse", glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
                // Setup specular color
                aiColor4D specular;
                if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
                    specular = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
                }
                materialProperties.emplace("specular", glm::vec4(specular.r, specular.g, specular.b, specular.a));
                // Setup shininess factor
                float shininess;
                if (AI_SUCCESS != aiMaterial->Get(AI_MATKEY_SHININESS, shininess)) {
                    shininess = 1.0f;
                }
                materialProperties.emplace("shininess", shininess);
                // Base color (albedo) textures
                uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_BASE_COLOR);
                for (uint32_t i = 0; i < textureCount; ++i) {
                    aiString texturePath;
                    if (AI_SUCCESS == aiMaterial->GetTexture(aiTextureType_BASE_COLOR, i, &texturePath)) {
                        if (texturePath.length <= 0) {
                            throw std::runtime_error("Could not read the texture for: " + matName);
                        }
                        textures.emplace("albedo" + std::to_string(i), TextureLoader::load(std::string(texturePath.C_Str())));
                    }
                }
                // Diffuse textures
                textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
                for (uint32_t i = 0; i < textureCount; ++i) {
                    aiString texturePath;
                    if (AI_SUCCESS == aiMaterial->GetTexture(aiTextureType_DIFFUSE, i, &texturePath)) {
                        if (texturePath.length <= 0) {
                            throw std::runtime_error("Could not read the texture for: " + matName);
                        }
                        textures.emplace("diffuse" + std::to_string(i), TextureLoader::load(std::string(texturePath.C_Str())));
                    }
                }
                // Specular textures
                textureCount = aiMaterial->GetTextureCount(aiTextureType_SPECULAR);
                for (uint32_t i = 0; i < textureCount; ++i) {
                    aiString texturePath;
                    if (AI_SUCCESS == aiMaterial->GetTexture(aiTextureType_SPECULAR, i, &texturePath)) {
                        if (texturePath.length <= 0) {
                            throw std::runtime_error("Could not read the texture for: " + matName);
                        }
                        textures.emplace("specular" + std::to_string(i), TextureLoader::load(std::string(texturePath.C_Str())));
                    }
                }
                // Normal map textures
                textureCount = aiMaterial->GetTextureCount(aiTextureType_NORMALS);
                for (uint32_t i = 0; i < textureCount; ++i) {
                    aiString texturePath;
                    if (AI_SUCCESS == aiMaterial->GetTexture(aiTextureType_NORMALS, i, &texturePath)) {
                        if (texturePath.length <= 0) {
                            throw std::runtime_error("Could not read the texture for: " + matName);
                        }
                        textures.emplace("normal" + std::to_string(i), TextureLoader::load(std::string(texturePath.C_Str())));
                    }
                }
                // Load all textures
                material = MaterialLoader::load(matName, "blinn_phong", materialProperties, textures);
            }
        }
    } else if (mesh->mMaterialIndex < materialOverrides.size()) {
        material = materialOverrides[mesh->mMaterialIndex];
    } else {
        material = MaterialLoader::load("debug");
    }
    if (!material) {
        throw std::runtime_error("No material has been provided for index: " + std::to_string(mesh->mMaterialIndex));
    }
    const std::shared_ptr<Mesh> loadedMesh = std::make_shared<Mesh>(vertices, indices, GL_TRIANGLES);
    loadedMeshes.emplace(getNodeName(mesh->mName, parent ? parent->name : ""), std::pair<std::shared_ptr<Material>, std::shared_ptr<Mesh>>(material, loadedMesh));
    return std::make_shared<MeshInstanceNode>(
        getNodeName(mesh->mName, parent ? parent->name : ""),
        loadedMesh, 
        material,
        Transform(),
        parent);
}

std::shared_ptr<SceneNode> MeshLoader::processNode(aiNode* node, const aiScene* scene, const std::vector<std::shared_ptr<Material>>& materialOverrides, const std::shared_ptr<SceneNode>& parent) {
    // Create current node as empty
    const std::shared_ptr<SceneNode> currentNode = std::make_shared<SceneNode>(
        getNodeName(node->mName, parent ? parent->name : ""),
        Transform(mat4ToGlm(node->mTransformation)), 
        parent
    );
    // Process all the node's meshes
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        currentNode->addChild(processMesh(mesh, scene, materialOverrides, currentNode));
    }
    // Process all the node's children
    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        currentNode->addChild(processNode(node->mChildren[i], scene, materialOverrides, currentNode));
    }
    return currentNode;
}

std::shared_ptr<SceneNode> MeshLoader::loadMesh(const std::string& fileName, const Transform& rootTransform, const std::vector<std::shared_ptr<Material>>& materialOverrides) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_GenSmoothNormals | aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Failed to open mesh file: " + fileName);
	}
    // Setup base template variables (in case they are not set in obj file)
    currentFile = fileName;
    currentNodeIndex = 0;
    // Create object tree from file
    const std::shared_ptr<SceneNode> rootNode = processNode(scene->mRootNode, scene, materialOverrides);
    // Set root node position to transform
    rootNode->setPosition(rootTransform.getPosition());
    rootNode->setRotation(rootTransform.getRotation());
    rootNode->setScale(rootTransform.getScale());
    // Free memory and return
    importer.FreeScene();
	return rootNode;
}