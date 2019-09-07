#include "gapi.hpp"
#include "gapi_vulkan_glfw.hpp"
#include "platform.hpp"
#include "platform_glfw.hpp"
#include <vulkan/vulkan.h>

Result<GApiContext> gapiCreateContext(Platform platform, Window window) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    GApiContext gapiContext = {};
    return resultCreateSuccess(gapiContext);
}

void gapiSwapWindow(Platform platform, Window window) {
}

void gapiShutdown(GApiContext context) {
    // vkDestroyInstance(instance, nullptr);
}
