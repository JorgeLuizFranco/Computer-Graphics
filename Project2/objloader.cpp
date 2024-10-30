#include "objloader.hpp"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool loadOBJ(const std::string& path,
             std::vector<glm::vec3>& out_vertices,
             std::vector<glm::vec2>& out_uvs,
             std::vector<glm::vec3>& out_normals) {
    Assimp::Importer importer;

    // Read the model file
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // Process the root node recursively
    std::vector<aiMesh*> meshes;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        meshes.push_back(scene->mMeshes[i]);
    }

    for (const auto& mesh : meshes) {
        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec3 vertex;
            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;
            out_vertices.push_back(vertex);

            // Process texture coordinates
            if (mesh->mTextureCoords[0]) { // Check if texture coordinates exist
                glm::vec2 uv;
                uv.x = mesh->mTextureCoords[0][i].x;
                uv.y = mesh->mTextureCoords[0][i].y;
                out_uvs.push_back(uv);
            } else {
                out_uvs.push_back(glm::vec2(0.0f, 0.0f)); // Default UVs
            }

            // Process normals
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            out_normals.push_back(normal);
        }
    }

    return true;
}
