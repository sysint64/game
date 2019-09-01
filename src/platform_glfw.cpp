#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Game", nullptr, nullptr);

    if (!window) {
        return resultCreateError<Platform>(
            "create_window",
            "glfwCreateWindow() error"
        );
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    Platform platform = {
        .window = window
    };

    if (glewInit() != GLEW_OK) {
        return resultCreateError<Platform>(
            "gl_init",
            "glewInit() error"
        );
    }

    initGL();

    return resultCreateSuccess(platform);
}

bool platformEventLoop(Platform platform) {
    int width, height;
    glfwGetFramebufferSize(platform.window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(platform.window);
    glfwPollEvents();
    return !glfwWindowShouldClose(platform.window);
}

float platformGetTicks() {
    return (float) glfwGetTime();
}

void platformSwapWindow(Platform platform) {
}

void platformShutdown(Platform platform) {
    glfwDestroyWindow(platform.window);
    glfwTerminate();
}

void initGL() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
