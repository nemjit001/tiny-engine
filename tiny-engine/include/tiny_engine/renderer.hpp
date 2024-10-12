#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace tiny_engine
{
    class Application;

    class Renderer
    {
    public:
        Renderer(GLFWwindow* pWindow, Application* pApplication);
        ~Renderer();

        Renderer(Renderer const&) = delete;
        Renderer& operator=(Renderer const&) = delete;

    public:
        VkInstance instance = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
    };
} // namespace tiny_engine
