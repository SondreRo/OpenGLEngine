#include <AssetManager.h>
#include <exception>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "Application.h"

bool AssetManager::CopyFileToContent(std::string OgPath, AssetType inAssetType)
{
    std::filesystem::path sourceFile = OgPath;
    std::filesystem::path targetParent;

    if (inAssetType == AssetType::MeshType)
    {
        targetParent = SOURCE_DIRECTORY("Content/Meshes");
    }
    else if (inAssetType == AssetType::TextureType)
    {
        targetParent = SOURCE_DIRECTORY("Content/Textures");

    }

    auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".

    try // If you want to avoid exception handling, then use the error code overload of the following functions.
    {
        std::filesystem::create_directories(targetParent); // Recursively create target directory if not existing.
        std::filesystem::copy_file(sourceFile, target, std::filesystem::copy_options::overwrite_existing);
        std::cout << "Copying file into: " << targetParent << std::endl;
    }
    catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
    {
        std::cout << e.what();
    }

	return true;

}

void AssetManager::CreateFileFromContentFolder()
{
    std::cout << "Reading FILE " << std::endl << std::endl;
    std::string path = SOURCE_DIRECTORY("Content/Meshes");
    std::ofstream MyFile(SOURCE_DIRECTORY("Content/index.txt"));

    if (MyFile.is_open())
    {
        MyFile << "Index for files. First Number is Index, second space is Name and extention, third is original file path" << std::endl;
    }
    int Index = 0;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string PathToObject = entry.path().string();
        std::string NameTest = PathToObject.substr(PathToObject.find_last_of("\\") + 1);
        std::cout << NameTest << std::endl;
        //std::cout << entry.path() << std::endl;
          // Create and open a text file
        if (MyFile.is_open())
        {
            MyFile << Index << " "<< NameTest << " " << "HERE GOES OG PATH" << std::endl;
        }
        Index++;
    }
    MyFile.close();
}

void AssetManager::CreateIndexWhenImporting(Mesh* inMesh)
{
    std::ofstream out;
    out.open(SOURCE_DIRECTORY("Content/index.txt"), std::ios::app);
    
    if (out.is_open())
    {
        out << 1 << " " << inMesh->DisplayName << " " << inMesh->path << std::endl;
    }
    out.close();
}
