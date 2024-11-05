#include "jApp.h"
namespace HUAR
{
    void App::Run()
    {
        while (!window.ShouldClose())
        {
            glfwPollEvents();
        }
    }
}
