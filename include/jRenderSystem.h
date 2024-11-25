#pragma once

#include "jDevice.h"
#include "jGameObject.h" // Ensure this header defines JinGameObject
#include "jPipeline.h"
#include "jCamera.h"

#include <memory>
#include <vector>

namespace HUAR{
class JinRenderSystem{
public:
    JinRenderSystem(JinDevice &device, VkRenderPass renderPass);
    ~JinRenderSystem();

    JinRenderSystem(const JinRenderSystem &) = delete;
    JinRenderSystem &operator=(const JinRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, 
                            std::vector<JinGameObject> &gameObjects,
                            const JinCamera &camera);
private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    JinDevice &device;

    VkPipelineLayout pipelineLayout;
    std::unique_ptr<JinPipeline> pipeline;
};
}