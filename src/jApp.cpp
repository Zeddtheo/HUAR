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
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto model01 = std::make_shared<JinModel>(device, vertices);
        std::vector<glm::vec3> colors{
            {1.f, .7f, .73f},
            {1.f, .87f, .73f},
            {1.f, 1.f, .73f},
            {.73f, 1.f, .8f},
            {.73, .88f, 1.f}  
        };
        for (auto& color : colors) {
            color = glm::pow(color, glm::vec3{2.2f});
        }
        for (int i = 0; i < 40; i++) {
            auto triangle = JinGameObject::createGameObject();
            triangle.model = model01;
            triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
            triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
            triangle.color = colors[i % colors.size()];
            gameObjects.push_back(std::move(triangle));
        }
    }
}
