#pragma once

#include <stdint.h>
#include <float.h>
#include <stddef.h>
#include "string.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef i32 b32;
typedef i64 b64;

typedef float f32;
typedef double f64;

typedef uintptr_t umm;
typedef intptr_t imm;

typedef size_t MemoryIndex;

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

enum class ResultCase {
    error,
    success
};

struct Error {
    char code[60];
    char message[256];
};

template<typename T>
struct Result {
    ResultCase resultCase;
    T payload;
    Error error;
};

template<typename T>
inline Result<T> resultCreateError(const Error error) {
    Result<T> result;
    result.resultCase = ResultCase::error;
    result.error = error;
    return result;
}

template<typename T>
inline Result<T> resultCreateError(const char* errorCode, const char* errorMessage) {
    Error error;
    strncpy(error.code, errorCode, 60);
    strncpy(error.message, errorMessage, 100);
    return resultCreateError<T>(error);
}

template<typename T>
inline Result<T> resultCreateSuccess(T payload) {
    Result<T> result;
    result.resultCase = ResultCase::success;
    result.payload = payload;
    return result;
}

template<typename T>
inline bool resultHasError(Result<T> result) {
    return result.resultCase == ResultCase::error;
}

template<typename T>
inline bool resultIsSuccess(Result<T> result) {
    return result.resultCase == ResultCase::success;
}
