#include "core/renderer.hpp"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "engine_config.h"
#include "core/application.hpp"
#include "core/assert.hpp"
#include "core/error_handling.hpp"

namespace tiny_engine
{
    Renderer::Renderer(GLFWwindow* pWindow, Application* pApplication)
    {
        TINY_ENGINE_ASSERT(pWindow != nullptr);
        TINY_ENGINE_ASSERT(pApplication != nullptr);

        uint32_t windowExtCount = 0;
        char const** windowExtensions = glfwGetRequiredInstanceExtensions(&windowExtCount);

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.applicationVersion = 0; // TODO(nemjit001): load app version from application interface
        appInfo.pApplicationName = pApplication->name();
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, TINY_ENGINE_VERSION_MAJOR, TINY_ENGINE_VERSION_MINOR, TINY_ENGINE_VERSION_PATCH);
        appInfo.pEngineName = TINY_ENGINE_NAME;
        appInfo.apiVersion = VK_API_VERSION_1_3;

        // TODO(nemjit001): Load extensions & layers from application (store function pointers into dispatch table)
        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledExtensionCount = windowExtCount;
        instanceCreateInfo.ppEnabledExtensionNames = windowExtensions;
        instanceCreateInfo.pNext = nullptr;

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
            TINY_ENGINE_DIE("Failed to create Vulkan instance");
        }

        if (glfwCreateWindowSurface(instance, pWindow, nullptr, &surface) != VK_SUCCESS) {
            TINY_ENGINE_DIE("Failed to create Vulkan surface");
        }
    }

    Renderer::~Renderer()
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }
} // namespace tiny_engine
