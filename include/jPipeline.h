#pragma once
#include "jDevice.h"
#include <string>
#include <vector>

namespace HUAR
{
    struct PipelineConfigInfo{
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    
    class JinPipeline
    {
    public:
        JinPipeline(JinDevice &device,
        const std::string &vertexShaderFile, 
        const std::string &fragmentShaderFile,
        const PipelineConfigInfo &configInfo);
        ~JinPipeline();

        JinPipeline(const JinPipeline &) = delete;
        JinPipeline &operator=(const JinPipeline &) = delete;

        std::vector<char> readFile(const std::string &filename);
        void createGraphicsPipeline(const std::string &vertexShaderFile, const std::string &fragmentShaderFile, const PipelineConfigInfo &configInfo);
        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        void bind(VkCommandBuffer commandBuffer);
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    private:
        JinDevice &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
    };
}