#pragma once

struct GApi;

struct GApiContext;

#ifdef GAPI_OPENGL
#include "gapi_opengl.hpp"

    #ifdef PLATFORM_SDL2
    #include "gapi_opengl_sdl2.hpp"
    #endif

    #ifdef PLATFORM_GLFW
    #include "gapi_opengl_glfw.hpp"
    #endif

#endif

#ifdef GAPI_VULKAN
#include "gapi_vulkan.hpp"

    #ifdef PLATFORM_SDL2
    #include "gapi_vulkan_sdl2.hpp"
    #endif

    #ifdef PLATFORM_GLFW
    #include "gapi_vulkan_glfw.hpp"
    #endif

#endif

#include "game_types.hpp"
#include "platform.hpp"

void gapiClear(float r, float g, float b);

Result<GApiContext> gapiCreateContext(Platform platform, Window window);

Result<GApi> gapiInit();

void gapiSwapWindow(Platform platform, Window window);

void gapiShutdown(GApiContext context);
