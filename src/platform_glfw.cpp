#include <cstdio>

#include "errors.hpp"
#include "gapi.hpp"
#include "platform.hpp"
#include "platform_glfw.hpp"

void initGL();

void errorCallback(int error, const char* description) {
    // TODO: log
    fprintf(stderr, "Error: %s\n", description);
}

Result<Platform> platformInit() {
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        return resultCreateGeneralError<Platform>(
            ErrorCode::PLATFORM_INIT,
            "glfwInit() error"
        );
    }

    auto initGapiResult = gapiInit();

    if (resultHasError(initGapiResult)) {
        return switchError<Platform>(initGapiResult);
    }

    Platform platform = {
        .gapi = getResultPayload(initGapiResult)
    };

    return resultCreateSuccess(platform);
}

Result<Window> platformCreateWindow(Platform platform) {
    GLFWwindow* glfwWindow = glfwCreateWindow(1024, 768, "Game", nullptr, nullptr);

    if (!glfwWindow) {
        return resultCreateGeneralError<Window>(
            ErrorCode::CREATE_WINDOW,
            "glfwCreateWindow() error"
        );
    }

    Window window = {
        .glfwWindow = glfwWindow
    };

    auto createContextResult = gapiCreateContext(platform, window);

    if (resultHasError(createContextResult)) {
        return switchError<Window>(createContextResult);
    }

    window.gapiContext = getResultPayload(createContextResult);

    return resultCreateSuccess(window);
}

bool platformEventLoop(Platform platform, Window window) {
    gapiClear(0, 0, 0);
    gapiSwapWindow(platform, window);
    glfwPollEvents();

    return !glfwWindowShouldClose(window.glfwWindow);
}

float platformGetTicks() {
    return (float) glfwGetTime();
}

void platformSwapWindow(Platform platform) {
}

void platformDestroyWindow(Window window) {
    gapiShutdown(window.gapiContext);
    glfwDestroyWindow(window.glfwWindow);
}

void platformShutdown(Platform platform) {
    glfwTerminate();
}
