#include "jWindow.h"
#include <stdexcept>

namespace HUAR{
    JinWindow::JinWindow(int width, int height, std::string title): width{width}, height{height}, title{title}{
        InitWindow();
    }

    JinWindow::~JinWindow(){
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    void JinWindow::InitWindow(){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(glfwWindow, this);
        glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);
    }

    void JinWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
        if(glfwCreateWindowSurface(instance, glfwWindow, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    void JinWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height){
        auto appWindow = reinterpret_cast<JinWindow*>(glfwGetWindowUserPointer(window));
        appWindow->framebufferResized = true;
        appWindow->width = width;
        appWindow->height = height;
    }
}