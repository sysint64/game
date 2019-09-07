#pragma once

#include "gapi.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Platform {
    GApi gapi;
};

struct Window {
    GLFWwindow* glfwWindow;
    GApiContext gapiContext;
};
