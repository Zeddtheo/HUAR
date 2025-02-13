#include "jRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace HUAR{
struct SimplePushConstantData{
    glm::mat4 transform{1.f};
    alignas(16) glm::vec3 color;
};

JinRenderSystem::JinRenderSystem(JinDevice &device, VkRenderPass renderPass)
    : device{device}
{
    createPipelineLayout();
    createPipeline(renderPass);
}

JinRenderSystem::~JinRenderSystem()
{
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void JinRenderSystem::createPipelineLayout(){
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
    if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void JinRenderSystem::createPipeline(VkRenderPass renderPass){
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    JinPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<JinPipeline>(device, 
                                            "../../shaders/simple_shader.vert.spv", 
                                            "../../shaders/simple_shader.frag.spv", 
                                            pipelineConfig);
}

void JinRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<JinGameObject> &gameObjects, const JinCamera &camera){
    pipeline->bind(commandBuffer);
    auto projectionView = camera.getProjection() * camera.getView();
    for (auto& obj : gameObjects) {
        //obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.001f, glm::two_pi<float>());
        //obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.0005f, glm::two_pi<float>());
        SimplePushConstantData push{};
        push.color = obj.color;
        push.transform = projectionView * obj.transform.mat4();

        vkCmdPushConstants(commandBuffer,
                            pipelineLayout,
                            VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT,
                            0,
                            sizeof(SimplePushConstantData),
                            &push);
        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}
}