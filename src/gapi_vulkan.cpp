#include "gapi.hpp"
#include "gapi_vulkan.hpp"
#include <vulkan/vulkan.h>

Result<GApi> gapiInit() {
    return resultCreateSuccess(GApi());
}

void gapiClear(float r, float g, float b) {
}
