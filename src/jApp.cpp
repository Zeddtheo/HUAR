#include "jApp.h"
#include <array>
#include <stdexcept>
namespace HUAR
{
    JinApp::JinApp()
    {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    JinApp::~JinApp()
    {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void JinApp::Run()
    {
        while (!window.ShouldClose())
        {
            glfwPollEvents();
            drawFrames();
        }

        vkDeviceWaitIdle(device.device());
    }

    void JinApp::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void JinApp::createPipeline()
    {
        auto pipelineConfig = JinPipeline::defaultPipelineConfigInfo(swapchain.width(), swapchain.height());
        pipelineConfig.renderPass = swapchain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<JinPipeline>(device, "C:/MISC/projects/HUAR/shaders/simple_shader.vert.spv", "C:/MISC/projects/HUAR/shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void JinApp::createCommandBuffers(){
        commandBuffers.resize(swapchain.imageCount());

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = device.getCommandPool();
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(device.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        for(int i = 0; i < commandBuffers.size(); i++){
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
                throw std::runtime_error("Failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapchain.getRenderPass();
            renderPassInfo.framebuffer = swapchain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapchain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            pipeline->bind(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
                throw std::runtime_error("Failed to record command buffer!");
            }
        }
    }

    void JinApp::drawFrames(){
        uint32_t imageIndex;
        auto result = swapchain.acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("Failed to acquire next image!");
        }

        result = swapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to submit command buffer!");
        }
    }
}
