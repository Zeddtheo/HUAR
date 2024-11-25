#pragma once

#include "jDevice.h"
#include "jSwapchain.h"
#include "jWindow.h"

#include <memory>
#include <vector>
#include <cassert>

namespace HUAR{
class JinRenderer{
public:
    JinRenderer(JinWindow &window, JinDevice &device);
    ~JinRenderer();

    JinRenderer(const JinRenderer &) = delete;
    JinRenderer &operator=(const JinRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return rSwapchain->getRenderPass(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    float getAspectRatio() const { return rSwapchain->extentAspectRatio(); }
    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress.");
        return rCommandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress.");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    JinWindow &window;
    JinDevice &device;

    std::unique_ptr<JinSwapchain> rSwapchain;
    std::vector<VkCommandBuffer> rCommandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex{0};
    bool isFrameStarted{false};
};
} // namespace HUAR
