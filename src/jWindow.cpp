#include "jWindow.h"
#include <stdexcept>

namespace HUAR{
    JinWindow::JinWindow(int width, int height, std::string title): width{width}, height{height}, title{title}{
        InitWindow();
    }

    JinWindow::~JinWindow(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void JinWindow::InitWindow(){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }

    void JinWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("Failed to create window surface!");
        }
    }
}