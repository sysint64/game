#include "gapi.hpp"

#include <GL/glew.h>

Result<GApi> gapiInit() {
    if (glewInit() != GLEW_OK) {
        return resultCreateError<GApi>(
            "gl_init",
            "glewInit() error"
        );
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return resultCreateSuccess(GApi());
}

void gapiClear(float r, float g, float b) {
    glClearColor(r, g, b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
