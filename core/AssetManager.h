#pragma once
#include <string>

#include "Mesh.h"


enum AssetType { MeshType, TextureType};


class AssetManager {

public:
	static bool CopyFileToContent(std::string OgPath, AssetType inAssetType);

	static void CreateFileFromContentFolder();
	static void CreateIndexWhenImporting(Mesh* inMesh);
};
