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
