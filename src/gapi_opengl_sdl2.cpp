#include "gapi.hpp"
#include "gapi_opengl_sdl2.hpp"
#include "platform.hpp"
#include "platform_sdl2.hpp"

#include <SDL2/SDL.h>

Result<GApiContext> gapiCreateContext(Platform platform) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    auto glContext = SDL_GL_CreateContext(platform.window);

    if (glContext == nullptr) {
        return resultCreateError<GApiContext>(
            "create_opengl_context",
            SDL_GetError()
        );
    }

    SDL_GL_MakeCurrent(platform.window, glContext);
    SDL_GL_SwapWindow(platform.window);

    GApiContext gapiContext = {
        .glContext = glContext
    };

    return resultCreateSuccess(gapiContext);
}

void gapiSwapWindow(Platform platform) {
    SDL_GL_SwapWindow(platform.window);
}

void gapiShutdown(GApiContext context) {
    SDL_GL_DeleteContext(context.glContext);
}
