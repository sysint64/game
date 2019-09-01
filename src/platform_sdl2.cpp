#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "platform_sdl2.hpp"

#include "platform.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>

void initGL();

Result<Platform> platformInit() {
    // Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return resultCreateError<Platform>(
            "platform_init",
            SDL_GetError()
        );
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    // Create window

    auto window = SDL_CreateWindow(
        "Game", // TODO(Andrey): rm hardcode
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024, // TODO(Andrey): rm hardcode
        768, // TODO(Andrey): rm hardcode
        SDL_WINDOW_OPENGL
    );

    // TODO(Andrey): uncomment
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    if (window == nullptr) {
        return resultCreateError<Platform>(
            "create_window",
            SDL_GetError()
        );
    }

    auto glContext = SDL_GL_CreateContext(window);

    if (glContext == nullptr) {
        return resultCreateError<Platform>(
            "create_opengl_context",
            SDL_GetError()
        );
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SwapWindow(window);
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        return resultCreateError<Platform>(
            "gl_init",
            "glewInit() error"
        );
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    initGL();

    Platform platform = {
        .window = window,
        .glContext = glContext,
    };

    return resultCreateSuccess(platform);
}

static bool showDemoWindow = true;
static bool showAnotherWindow = false;
static ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool platformEventLoop(Platform platform) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
            return false;
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(platform.window);
    ImGui::NewFrame();

    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &showDemoWindow);        // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*) &clearColor); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (showAnotherWindow) {
        ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");

        if (ImGui::Button("Close Me")) {
            showAnotherWindow = false;
        }

        ImGui::End();
    }

    // Rendering
    ImGui::Render();

    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    SDL_GL_SwapWindow(platform.window);

    return true;
}

float platformGetTicks() {
    return SDL_GetTicks();
}

void platformSwapWindow(Platform platform) {
    SDL_GL_SwapWindow(platform.window);
}

void platformShutdown(Platform platform) {
    SDL_GL_DeleteContext(platform.glContext);
    SDL_DestroyWindow(platform.window);
    SDL_Quit();
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
