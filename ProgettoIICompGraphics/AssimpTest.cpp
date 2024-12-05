#include "AssimpTest.hpp"

// TODO: change this file!!!

#include "Mesh.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>
#include <stdexcept>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static Mesh3D* processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex3D> vertices;
    std::vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex3D vertex;
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
    return new Mesh3D(vertices, indices, GL_TRIANGLES);
}

static void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh3D *>& meshesOutput) {
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

std::vector<Mesh3D *> importModel(const std::string& filePath) {
	std::vector<Mesh3D *> meshes;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_OptimizeMeshes | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error("Failed to open mesh file: " + filePath);
	}
    processNode(scene->mRootNode, scene, meshes);
	return meshes;
}