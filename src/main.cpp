#include "imgui.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

#include "platform.hpp"
#include "game.hpp"
#include "game_types.hpp"

#ifdef SINGLE_SOURCE
#include "single_source_link.hpp"
#endif

int main() {
    Game game;

    game.init();
    auto platformInitResult = platformInit();

    if (resultIsSuccess(platformInitResult)) {
        auto platform = resultGetPayload(platformInitResult);
        auto createWindowResult = platformCreateWindow(platform);

        if (resultIsSuccess(createWindowResult)) {
            auto window = resultGetPayload(createWindowResult);
            bool running = true;
            game.afterInit();

            while (running) {
                running = platformEventLoop(platform, window);
                game.gameMainLoop(platform, window);
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

    return 0;
}
