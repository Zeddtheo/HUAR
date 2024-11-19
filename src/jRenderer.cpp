#include "jRenderer.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace HUAR {
JinRenderer::JinRenderer(JinWindow &window, JinDevice &device)
    : window{window}, device{device} {
  recreateSwapChain();
  createCommandBuffers();
}

JinRenderer::~JinRenderer() { freeCommandBuffers(); }

void JinRenderer::recreateSwapChain() {
  auto extent = window.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(device.device());

  if (rSwapchain == nullptr) {
    rSwapchain = std::make_unique<JinSwapchain>(device, extent);
  } else {
    std::shared_ptr<JinSwapchain> oldSwapchain = std::move(rSwapchain);
    rSwapchain = std::make_unique<JinSwapchain>(device, extent, oldSwapchain);

    if (!oldSwapchain->compareSwapFormats(*rSwapchain)) {
      throw std::runtime_error("Swapchain image format has changed!");
    }
  }
}

void JinRenderer::createCommandBuffers() {
  rCommandBuffers.resize(JinSwapchain::MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(rCommandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo, rCommandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void JinRenderer::freeCommandBuffers() {
  vkFreeCommandBuffers(
      device.device(), 
      device.getCommandPool(), 
      static_cast<uint32_t>(rCommandBuffers.size()), 
      rCommandBuffers.data());
  rCommandBuffers.clear();
}

VkCommandBuffer JinRenderer::beginFrame(){
    assert(!isFrameStarted && "Can't call beginFrame while frame is in progress.");
    
    auto result = rSwapchain->acquireNextImage(&currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    
    isFrameStarted = true;
    
    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    
    return commandBuffer;
}

void JinRenderer::endFrame(){
    assert(isFrameStarted && "Can't call endFrame while frame is not in progress.");
    auto commandBuffer = getCurrentCommandBuffer();
    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
        throw std::runtime_error("failed to record command buffer!");
    }
    auto result = rSwapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()){
        window.resetWindowResizedFlag();
        recreateSwapChain();
    }else if(result != VK_SUCCESS){
        throw std::runtime_error("failed to present swap chain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % JinSwapchain::MAX_FRAMES_IN_FLIGHT;
}

void JinRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
    assert(isFrameStarted && "Can't begin render pass when frame is not in progress.");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame.");
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = rSwapchain->getRenderPass();
    renderPassInfo.framebuffer = rSwapchain->getFrameBuffer(currentImageIndex);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = rSwapchain->getSwapChainExtent();
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(rSwapchain->width());
    viewport.height = static_cast<float>(rSwapchain->height());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, rSwapchain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void JinRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
    assert(isFrameStarted && "Can't end render pass when frame is not in progress.");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame.");
    vkCmdEndRenderPass(commandBuffer);
}

} // namespace HUAR