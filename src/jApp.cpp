#include "jApp.h"
#include "jRenderSystem.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <array>
#include <stdexcept>
#include <cassert>
namespace HUAR
{
    JinApp::JinApp()
    {
        loadGameObjects();
    }

    void JinApp::Run()
    {
        JinRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        while (!window.ShouldClose())
        {
            glfwPollEvents();
            if(auto commandBuffer = renderer.beginFrame()){
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void JinApp::loadGameObjects(){
        std::vector<JinModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        auto model = std::make_shared<JinModel>(device, vertices);

        auto triangle = JinGameObject::createGameObject();
        triangle.model = model;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }
}
