#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
        return resultCreateError<Platform>(
            "platform_init",
            "glfwInit() error"
        );
    }

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Game", nullptr, nullptr);

    if (!window) {
        return resultCreateError<Platform>(
            "create_window",
            "glfwCreateWindow() error"
        );
    }

    Platform platform = {
        .window = window
    };

    auto createContextResult = gapiCreateContext(platform);

    if (resultHasError(createContextResult)) {
        // return mapError<Platform>(createContextResult);
    }

    platform.gapiContext = getResultPayload(createContextResult);

    auto initGapiResult = gapiInit();

    if (resultHasError(initGapiResult)) {
    //     return mapError<Platform>(initGapiResult);
    }

    // if (glewInit() != GLEW_OK) {
    //     return resultCreateError<Platform>(
    //         "gl_init",
    //         "glewInit() error"
    //     );
    // }

    // initGL();

    return resultCreateSuccess(platform);
}

bool platformEventLoop(Platform platform) {
    // int width, height;
    // glfwGetFramebufferSize(platform.window, &width, &height);
    // glViewport(0, 0, width, height);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glfwSwapBuffers(platform.window);
    gapiClear(0, 0, 0);
    gapiSwapWindow(platform);
    glfwPollEvents();

    return !glfwWindowShouldClose(platform.window);
}

float platformGetTicks() {
    return (float) glfwGetTime();
}

void platformSwapWindow(Platform platform) {
}

void platformShutdown(Platform platform) {
    gapiShutdown(platform.gapiContext);
    glfwDestroyWindow(platform.window);
    glfwTerminate();
}
