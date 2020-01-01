#pragma once

enum class ErrorCode {
    // Platform
    ALLOCATION_ERROR,
    PLATFORM_INIT,
    CREATE_WINDOW,
    LOAD_ASSET,

    // GAPI
    GAPI_INIT,
    GAPI_CREATE_CONTEXT,
};
