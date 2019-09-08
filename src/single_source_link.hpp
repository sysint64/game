#ifdef PLATFORM_SDL2
#include "platform_sdl2.cpp"
#endif

#ifdef PLATFORM_GLFW
#include "platform_glfw.cpp"
#endif

#ifdef GAPI_OPENGL
#include "gapi_opengl.cpp"

    #ifdef PLATFORM_SDL2
    #include "gapi_opengl_sdl2.cpp"
    #endif

    #ifdef PLATFORM_GLFW
    #include "gapi_opengl_glfw.cpp"
    #endif

#endif

#ifdef GAPI_VULKAN
#include "gapi_vulkan.cpp"

    #ifdef PLATFORM_SDL2
    #include "gapi_vulkan_sdl2.cpp"
    #endif

    #ifdef PLATFORM_GLFW
    #include "gapi_vulkan_glfw.cpp"
    #endif

#endif

#ifdef __linux__
#include "platform_linux.cpp"
#endif

#include "gapi.cpp"
#include "game.cpp"
