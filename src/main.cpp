#include "imgui.h"

#include "stdio.h"
#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <iostream>

#include "platform.hpp"

#ifdef PLATFORM_SDL2
#include "platform_sdl2.hpp"
#endif

#include "game.hpp"
#include "game_types.hpp"

int main() {
    bool running = true;
    auto platformInitResult = platformInit();

    if (initGameState()) {
        if (resultIsSuccess(platformInitResult)) {
            auto platform = platformInitResult.payload;

            while (running) {
                running = platformEventLoop(platform);
            }

            platformShutdown(platform);
        }
        else {
            // TODO(Andrey): log
            puts(platformInitResult.error.message);
        }
    }
    else {
        // TODO(Andrey): log
        puts("Game state initialization error");
    }

    return 0;
}
