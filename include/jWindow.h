#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace HUAR{
    class Window{
    public:
        Window(int width, int height, std::string title);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        inline bool ShouldClose(){
            return glfwWindowShouldClose(window);
        }
    private:        
        const int width;
        const int height;
        std::string title;
        GLFWwindow* window;

        void InitWindow();
    };
}