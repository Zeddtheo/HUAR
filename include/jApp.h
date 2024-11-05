#pragma once

#include "jWindow.h"
#include "jPipeline.h"

namespace HUAR
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void Run();
    private:
        Window window{WIDTH, HEIGHT, "Vulkan"};
        Pipeline pipeline{"shaders/shader.vert.spv", "shaders/shader.frag.spv"};
    };
}