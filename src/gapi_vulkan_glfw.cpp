#include "gapi.hpp"
#include "gapi_vulkan_glfw.hpp"
#include "platform.hpp"
#include "platform_glfw.hpp"
#include "game_types.hpp"
#include <vulkan/vulkan.h>
#include <vector>

#define VALIDATION_LAYERS_COUNT 1

const char* validationLayers[VALIDATION_LAYERS_COUNT] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

static Result<VkResult> createInstance(GApiContext* gapiContext);

static Result<GApiContext> initVulkan();

static bool checkValidationLayerSupport();

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

    if (resultHasError(result)) {
        return switchError<GApiContext>(result);
    }

    if (resultGetPayload(result) != VK_SUCCESS) {
        return resultCreateGeneralError<GApiContext>(
            ErrorCode::GAPI_CREATE_CONTEXT,
            "vkCreateInstance failed with code: %d", result
        );
    }

    return resultCreateSuccess(gapiContext);
}

static Result<VkResult> createInstance(GApiContext* gapiContext) {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        return resultCreateGeneralError<VkResult>(
            ErrorCode::GAPI_CREATE_CONTEXT,
            "Validation layers requested, but not available!"
        );
    }

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

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = VALIDATION_LAYERS_COUNT;
        createInfo.ppEnabledLayerNames = validationLayers;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    createInfo.enabledLayerCount = 0;

    auto result = vkCreateInstance(&createInfo, nullptr, &gapiContext->instance);
    return resultCreateSuccess(result);
}

static bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}
