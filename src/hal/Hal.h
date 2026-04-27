/*
 * NeoCalculator - NumOS
 * Copyright (C) 2026 Juan Ramon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/**
 * Hal.h — Hardware Abstraction Layer
 *
 * Abstrae funciones que difieren entre ESP32 (Arduino) y PC (SDL2/native).
 * En código portable, usar HAL_Log/HAL_Delay/HAL_GetMillis en vez de
 * Serial.printf/delay/millis directamente.
 *
 * Definiciones:
 *   NATIVE_SIM  → Compilación nativa para PC (SDL2)
 *   ARDUINO     → Compilación para ESP32 (framework Arduino)
 */

#pragma once

#include <cstdint>

#ifdef NATIVE_SIM
    // ── PC: SDL2 + stdlib ──────────────────────────────────────────────
    #include <cstdio>
    #include <cstdarg>
    #include <SDL2/SDL.h>

    #define HAL_Log(fmt, ...)       printf("[SIM] " fmt "\n", ##__VA_ARGS__)
    #define HAL_Delay(ms)           SDL_Delay(ms)

    inline uint32_t HAL_GetMillis() { return SDL_GetTicks(); }

#elif defined(ARDUINO)
    // ── ESP32: Arduino framework ───────────────────────────────────────
    #include <Arduino.h>

    #define HAL_Log(fmt, ...)       Serial.printf(fmt "\n", ##__VA_ARGS__)
    #define HAL_Delay(ms)           delay(ms)

    inline uint32_t HAL_GetMillis() { return millis(); }

#else
    // ── Fallback: stdlib puro (sin SDL2 ni Arduino) ────────────────────
    #include <cstdio>
    #include <cstdarg>
    #include <chrono>
    #include <thread>

    #define HAL_Log(fmt, ...)       printf("[HAL] " fmt "\n", ##__VA_ARGS__)
    #define HAL_Delay(ms)           std::this_thread::sleep_for(std::chrono::milliseconds(ms))

    inline uint32_t HAL_GetMillis() {
        static auto t0 = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - t0).count();
    }

#endif
