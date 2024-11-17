#pragma once

#include "jWindow.h"
#include "jPipeline.h"
#include "jSwapchain.h"
#include "jDevice.h"
#include "jModel.h"
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
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrames();
        void recreateSwapchain();
        void recordCommandBuffer(int imageIndex);

        JinWindow window{WIDTH, HEIGHT, "Vulkan"};
        JinDevice device{window};
        std::unique_ptr<JinSwapchain> swapchain;
        std::unique_ptr<JinPipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<JinModel> model;
    };
}