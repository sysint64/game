#include "imgui.h"

#include "stdio.h"
#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <iostream>

#include "platform.hpp"
#include "game.hpp"
#include "game_types.hpp"

int main() {
    bool running = true;

    if (initGameState()) {
        auto platformInitResult = platformInit();

        if (resultIsSuccess(platformInitResult)) {
            auto platform = getResultPayload(platformInitResult);
            auto createWindowResult = platformCreateWindow(platform);

            if (resultIsSuccess(createWindowResult)) {
                auto window = getResultPayload(createWindowResult);

                while (running) {
                    running = platformEventLoop(platform, window);
                }

                platformDestroyWindow(window);
            }
            else {
                // TODO(Andrey): log
                puts(createWindowResult.error.message);
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
