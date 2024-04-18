#include <MeshImporter.h>
#include <iostream>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void MeshImporter::PrintTest()
{
	std::cout << "TEST FROM ASSIM" << std::endl;
}

Mesh* MeshImporter::ImportAssimp(std::string RelativePath)
{
    meshes.clear();
	std::cout << RelativePath << std::endl;
	Assimp::Importer import;


    unsigned int processFlags =
        aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
        aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
        aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
        aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
        //aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
        aiProcess_SortByPType | // ?
        aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
        aiProcess_RemoveRedundantMaterials | // remove redundant materials
        aiProcess_FindDegenerates | // remove degenerated polygons from the import
        aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
        aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
        aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
        aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
        aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
        aiProcess_OptimizeMeshes | // join small meshes, if possible;
        aiProcess_PreTransformVertices | //-- fixes the transformation issue.
        aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
        aiProcess_FlipUVs |


        aiProcess_FlipWindingOrder | // Flip winding order, midlertidig løsning.
        0;




	const aiScene * scene = import.ReadFile(RelativePath, processFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return nullptr;
	}
    directory = RelativePath.substr(0, RelativePath.find_last_of('/'));
	processNode(scene->mRootNode, scene);

    Mesh* toReturn = meshes[0];
    toReturn->DisplayName = scene->mMeshes[0]->mName.C_Str();
    toReturn->path = RelativePath;
    return toReturn;
}

void MeshImporter::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }


	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* MeshImporter::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vertex.mPosition.x = mesh->mVertices[i].x;
        vertex.mPosition.y = mesh->mVertices[i].y;
        vertex.mPosition.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.mNormal.x = mesh->mNormals[i].x;
            vertex.mNormal.y = mesh->mNormals[i].y;
            vertex.mNormal.z = mesh->mNormals[i].z;
        }
        else
        {
            vertex.mNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

      

        if (mesh->mTextureCoords[0])
        {
            vertex.mTexCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.mTexCoord.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.mTexCoord = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);

     



    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    Mesh* MeshToReturn = new Mesh(vertices, indices, textures);

    if (indices.size() > 0)
    {
        MeshToReturn->UseElements = true;

    }

    return MeshToReturn;
}



std::vector<Texture> MeshImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

unsigned int MeshImporter::TextureFromFile(std::string path)
{
    stbi_set_flip_vertically_on_load(false);
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    //unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);TestMaterial.png
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
