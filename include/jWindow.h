#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace HUAR{
    class JinWindow{
    public:
        JinWindow(int width, int height, std::string title);
        ~JinWindow();

        JinWindow(const JinWindow &) = delete;
        JinWindow &operator=(const JinWindow &) = delete;

        inline bool ShouldClose(){
            return glfwWindowShouldClose(window);
        }

        VkExtent2D getExtent(){
            return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    private:        
        const int width;
        const int height;
        std::string title;
        GLFWwindow* window;

        void InitWindow();
    };
}