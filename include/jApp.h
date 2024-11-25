#pragma once

#include "jWindow.h"
#include "jRenderer.h"
#include "jDevice.h"
#include "jGameObject.h"
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
        ~JinApp(){};

        JinApp(const JinApp &) = delete;
        JinApp &operator=(const JinApp &) = delete;

        void Run();
    private:
        void loadGameObjects();
        JinWindow window{WIDTH, HEIGHT, "Vulkan"};
        JinDevice device{window};
        JinRenderer renderer{window, device};
        std::vector<JinGameObject> gameObjects;
    };
}