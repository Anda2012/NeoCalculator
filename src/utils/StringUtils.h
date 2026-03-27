#pragma once

#include <cstddef>
#include <cstdarg>
#include <cstdio>

namespace utils {

inline void safe_strcpy(char* dst, const char* src, size_t size) {
    if (!dst || size == 0) {
        return;
    }
    if (!src) {
        dst[0] = '\0';
        return;
    }

    size_t i = 0;
    for (; i + 1 < size && src[i] != '\0'; ++i) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

inline int snprintf_append(char* dst, size_t cap, size_t& pos, const char* fmt, ...) {
    if (!dst || cap == 0 || pos >= cap) {
        return -1;
    }

    va_list args;
    va_start(args, fmt);
    int w = vsnprintf(dst + pos, cap - pos, fmt, args);
    va_end(args);

    if (w < 0) {
        return -1;
    }

    if (static_cast<size_t>(w) >= cap - pos) {
        // truncation; make pos point to last character.
        pos = cap - 1;
        return -1;
    }

    pos += static_cast<size_t>(w);
    return w;
}

} // namespace utils
