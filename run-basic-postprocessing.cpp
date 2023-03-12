#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/exporter.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

std::string sanitizeXFile(const std::string& filename) {
    // Read in the .x file
    std::ifstream infile(filename);
    std::stringstream buffer;
    buffer << infile.rdbuf();
    std::string file_contents = buffer.str();

    // Remove comments and blank lines
    std::regex comments_regex("//.*\n");
    std::regex blanklines_regex("\n\\s*\n");
    file_contents = std::regex_replace(file_contents, comments_regex, "");
    file_contents = std::regex_replace(file_contents, blanklines_regex, "\n");

    // Ensure proper formatting
    std::regex whitespace_regex("[ \t]+");
    file_contents = std::regex_replace(file_contents, whitespace_regex, " ");

    // Remove unsupported features, but don't remove if the line contains "3;0"
    //std::regex faces_regex("(?<!3;)3;(?!0)");
    ////std::regex faces_regex("3;");
    //file_contents = std::regex_replace(file_contents, faces_regex, "");

    // Ensure ASCII encoding
    std::regex encoding_regex("xof ([0-9.]+) ((utf)|(unicode))");
    file_contents = std::regex_replace(file_contents, encoding_regex, "xof $1 ascii");


    //realnumber Failed to load .X file: Cannot parse string  as a real number: does not start with digit or decimal point followed by digit.
    // Replace floating point numbers with valid values
    //std::regex float_regex("-?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
    //file_contents = std::regex_replace(file_contents, float_regex, "0.0");
        // Replace any invalid floating-point numbers with a valid default value


    // Write out the sanitized file
    std::string sanitized_filename = "sanitized_" + filename;
    std::ofstream outfile(sanitized_filename);
    outfile << file_contents;
    outfile.close();

    return sanitized_filename;
}

int main()
{
    // Sanitize the .x file
    std::string sanitized_filename = sanitizeXFile("input.x"); //it produces a bug of missing ;

    // Create an instance of the Importer class
    Assimp::Importer importer;

    
    // Specify all postprocessing flags to optimize the model
    unsigned int postFlags = aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate |
        aiProcess_RemoveComponent |
        aiProcess_GenNormals |
        aiProcess_GenSmoothNormals |
        aiProcess_SplitLargeMeshes |
        aiProcess_SortByPType |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_ImproveCacheLocality |
        aiProcess_LimitBoneWeights |
        aiProcess_OptimizeMeshes |
        aiProcess_PreTransformVertices |
        aiProcess_TransformUVCoords |
        aiProcess_FlipUVs |
        aiProcess_EmbedTextures |
        aiProcess_FindInstances |
        aiProcess_ValidateDataStructure |
        aiProcess_Debone |
        aiProcess_GlobalScale |
        aiProcess_FixInfacingNormals |
        aiProcess_RemoveRedundantMaterials |
        //aiProcess_ResolveGVCaches |
        //aiProcess_ResolveInvalidData |
        aiProcess_SplitByBoneCount |
        // aiProcess_SymmetricizeUVCoords |
        aiProcess_Triangulate;
    //aiProcess_UnifyVertices;
    
    //unsigned int postFlags = aiProcess_Triangulate;

    // Load the sanitized .x file from a file and apply all postprocessing steps
    
    const aiScene* scene = importer.ReadFile(sanitized_filename.c_str(), postFlags);
    //const aiScene* scene = importer.ReadFile("index.x", postFlags);

    if (!scene)
    {
        std::cerr << "Failed to load .X file: " << importer.GetErrorString() << std::endl;
        return 1;
    }

    // Create an instance of the Exporter class
    Assimp::Exporter exporter;

    // Find a suitable exporter for the .fbx format
    unsigned int formatCount = exporter.GetExportFormatCount();
    const aiExportFormatDesc* formatDesc = nullptr;
    for (unsigned int i = 0; i < formatCount; i++)
    {
        const aiExportFormatDesc* desc = exporter.GetExportFormatDescription(i);
        if (std::strcmp(desc->id, "fbx") == 0)
        {
            formatDesc = desc;
            break;
        }
    }

    if (!formatDesc)
    {
        std::cerr << "Failed to find .fbx exporter" << std::endl;
        return 1;
    }

    // Export the scene to a .fbx file
    aiReturn result = exporter.Export(scene, formatDesc->id, "output-advanced.fbx");

    if (result != aiReturn_SUCCESS)
    {
        std::cerr << "Failed to export .fbx file: " << exporter.GetErrorString() << std::endl;
        return 1;
    }

    std::cout << "Export successful!" << std::endl;

    return 0;
}