#pragma once

#include "jWindow.h"
#include "jPipeline.h"
#include "jSwapchain.h"
#include "jDevice.h"
#include <memory>
#include <vector>

namespace HUAR
{
    class JinApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        JinApp();
        ~JinApp();

        JinApp(const JinApp &) = delete;
        JinApp &operator=(const JinApp &) = delete;

        void Run();
    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrames();

        JinWindow window{WIDTH, HEIGHT, "Vulkan"};
        JinDevice device{window};
        JinSwapchain swapchain{device, window.getExtent()};
        std::unique_ptr<JinPipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}