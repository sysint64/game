#pragma once

#include "gapi.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Platform {
    GLFWwindow* window;
    GApiContext gapiContext;
};
