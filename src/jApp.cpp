#include "jApp.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <array>
#include <stdexcept>
#include <cassert>
namespace HUAR
{
    struct SimplePushConstantData{
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    
    JinApp::JinApp()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapchain();
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
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void JinApp::createPipeline()
    {
        assert(swapchain != nullptr && "Cannot create pipeline before swapchain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        // auto pipelineConfig = JinPipeline::defaultPipelineConfigInfo(swapchain.width(), swapchain.height());
        PipelineConfigInfo pipelineConfig{};
        JinPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = swapchain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<JinPipeline>(device, "C:/MISC/projects/HUAR/shaders/simple_shader.vert.spv", "C:/MISC/projects/HUAR/shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void JinApp::createCommandBuffers(){
        commandBuffers.resize(swapchain->imageCount());

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = device.getCommandPool();
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(device.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate command buffers!");
        }
    }

    void JinApp::drawFrames(){
        uint32_t imageIndex;
        auto result = swapchain->acquireNextImage(&imageIndex);

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            recreateSwapchain();
            return;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);
        result = swapchain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()){
            window.resetWindowResizedFlag();
            recreateSwapchain();
            return;
        }else if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to present swap chain image!");
        }
    }

    void JinApp::loadModels(){
        std::vector<JinModel::Vertex> vertices{
            {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
        };

        model = std::make_unique<JinModel>(device, vertices);
    }

    void JinApp::recreateSwapchain(){
        auto extent = window.getExtent();
        while(extent.width == 0 || extent.height == 0){
            extent = window.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.device());

        if(swapchain == nullptr){
            swapchain = std::make_unique<JinSwapchain>(device, extent);
        }else{
            swapchain = std::make_unique<JinSwapchain>(device, extent, std::move(swapchain));
            if(swapchain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        createPipeline();
    }

    void JinApp::freeCommandBuffers(){
        vkFreeCommandBuffers(device.device(),
                             device.getCommandPool(), 
                             static_cast<uint32_t>(commandBuffers.size()), 
                             commandBuffers.data());

        commandBuffers.clear();
    }

    void JinApp::recordCommandBuffer(int imageIndex){
        static int frame = 30;
        frame = (frame + 1) % 100;
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to begin recording command buffer!");
        }
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapchain->getRenderPass();
        renderPassInfo.framebuffer = swapchain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapchain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapchain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, swapchain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        pipeline->bind(commandBuffers[imageIndex]);
        model->bind(commandBuffers[imageIndex]);

        for(int j = 0; j < 4; j++){
            SimplePushConstantData push{};
            push.offset = {-0.5f + frame * 0.02f, -0.4f + j * 0.25f};
            push.color = {0.0f, 0.0f, 0.2f+j*0.2f};
            vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            model->draw(commandBuffers[imageIndex]);
        }   

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS){
            throw std::runtime_error("Failed to record command buffer!");
        }
    }
}
