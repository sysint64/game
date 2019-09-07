#include "gapi.hpp"
#include "gapi_opengl_glfw.hpp"
#include "platform.hpp"
#include "platform_glfw.hpp"

Result<GApiContext> gapiCreateContext(Platform platform, Window window) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    glfwMakeContextCurrent(window.glfwWindow);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        return resultCreateGeneralError<GApiContext>(
            ErrorCode::GAPI_INIT,
            "glewInit() error"
        );
    }

    GApiContext gapiContext = {};
    return resultCreateSuccess(gapiContext);
}

void gapiSwapWindow(Platform platform, Window window) {
    glfwSwapBuffers(window.glfwWindow);
}

void gapiShutdown(GApiContext context) {
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
