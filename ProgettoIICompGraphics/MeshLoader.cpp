#include "MeshLoader.hpp"

#include "Mesh.hpp"
#include "Vertex.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <stdexcept>

namespace MeshLoader {
    static Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    static void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh *>& meshesOutput);
}

Mesh* MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
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
    return new Mesh(vertices, indices, GL_TRIANGLES);
}

void MeshLoader::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh *>& meshesOutput) {
    // process all the node's meshes (if any)
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshesOutput.emplace_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, meshesOutput);
    }
}

std::vector<Mesh *> MeshLoader::loadMesh(const std::string& fileName) {
	std::vector<Mesh *> meshes;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_OptimizeMeshes | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Failed to open mesh file: " + fileName);
	}
    processNode(scene->mRootNode, scene, meshes);
    importer.FreeScene();
	return meshes;
}