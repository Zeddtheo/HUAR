#include "jPipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
namespace HUAR
{
    Pipeline::Pipeline(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
    {
       createGraphicsPipeline(vertexShaderFile, fragmentShaderFile);
    }

    Pipeline::~Pipeline()
    {
    }

    std::vector<char> Pipeline::readFile(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    void Pipeline::createGraphicsPipeline(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
    {
        auto vertexCode = readFile(vertexShaderFile);
        auto fragmentCode = readFile(fragmentShaderFile);

        std::cout << "Vertex Shader Size: " <<vertexCode.size() << std::endl;
        std::cout << "Fragment Shader Size: " << fragmentCode.size() <<std::endl;
    }
}
