#include "gapi.hpp"
#include "gapi_vulkan_glfw.hpp"
#include "platform.hpp"
#include "platform_glfw.hpp"
#include <vulkan/vulkan.h>

static VkResult createInstance(GApiContext* gapiContext);

static Result<GApiContext> initVulkan();

Result<GApiContext> gapiCreateContext(Platform platform, Window window) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return initVulkan();
}

void gapiSwapWindow(Platform platform, Window window) {
}

void gapiShutdown(GApiContext context) {
    vkDestroyInstance(context.instance, nullptr);
}

static Result<GApiContext> initVulkan() {
    GApiContext gapiContext = {};
    auto result = createInstance(&gapiContext);

    if (result != VK_SUCCESS) {
        return resultCreateGeneralError<GApiContext>(
            ErrorCode::GAPI_CREATE_CONTEXT,
            "vkCreateInstance failed with code: %d", result
        );
    }

    return resultCreateSuccess(gapiContext);
}

static VkResult createInstance(GApiContext* gapiContext) {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    return vkCreateInstance(&createInfo, nullptr, &gapiContext->instance);
}
