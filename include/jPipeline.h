#pragma once
#include <string>
#include <vector>

namespace HUAR
{
    class Pipeline
    {
    public:
        Pipeline(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
        ~Pipeline();

        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;

        std::vector<char> readFile(const std::string &filename);
        void createGraphicsPipeline(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
    private:

    };
}