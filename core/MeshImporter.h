#pragma once
#include <string>

#include <Mesh.h>
#include <Texture.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class MeshImporter {
public:
	void PrintTest();

	std::vector<Mesh*> ImportAssimp(std::string RelativePath);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(std::string path);

	std::vector<Mesh*> meshes;
	std::vector<Texture> textures_loaded;

	std::string directory;
};
