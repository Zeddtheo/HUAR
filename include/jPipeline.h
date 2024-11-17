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
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    private:
        JinDevice &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
    };
}