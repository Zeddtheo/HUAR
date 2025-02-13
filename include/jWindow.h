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
            return glfwWindowShouldClose(glfwWindow);
        }

        VkExtent2D getExtent(){
            return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        }

        bool wasWindowResized(){
            return framebufferResized;
        }

        void resetWindowResizedFlag(){
            framebufferResized = false;
        }
        GLFWwindow *getGLFWwindow() const { return glfwWindow; }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    private:        
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        int width;
        int height;
        bool framebufferResized = false;
        
        std::string title;
        GLFWwindow* glfwWindow;

        void InitWindow();
    };
}