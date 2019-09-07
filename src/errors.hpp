#pragma once

enum class ErrorCode {
    // Platform
    ALLOCATION_ERROR,
    PLATFORM_INIT,
    CREATE_WINDOW,

    // GAPI
    GAPI_INIT,
    GAPI_CREATE_CONTEXT,
};
