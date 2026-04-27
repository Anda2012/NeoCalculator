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
 * ColorUtils.h — Shared RGB colour-space conversion utilities
 *
 * Provides an inline constexpr RGB888 → RGB565 converter used by
 * GrapherApp, GraphView, NeuralLabApp, OpticsLabApp, and any future
 * modules that push pixels into an RGB565 buffer (Kandinsky pattern).
 *
 * Usage:
 *   #include "../utils/ColorUtils.h"
 *   uint16_t c = utils::rgb888to565(0xFF8800);
 */
#pragma once

#include <cstdint>

namespace utils {

/// Convert 24-bit RGB888 (0xRRGGBB) to 16-bit RGB565.
/// Accepted as constexpr so the compiler can fold constant colours at build
/// time (zero runtime cost for palette definitions).
inline constexpr uint16_t rgb888to565(uint32_t rgb) {
    return static_cast<uint16_t>(
        (((rgb >> 19) & 0x1F) << 11) |   // R: top 5 bits
        (((rgb >> 10) & 0x3F) <<  5) |   // G: top 6 bits
        (((rgb >>  3) & 0x1F))            // B: top 5 bits
    );
}

} // namespace utils
