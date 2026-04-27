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

#pragma once

#include <stdint.h>
#include <lvgl.h>

namespace ui {

// Checks critical glyph coverage and reports baseline-related metrics in Serial.
bool runStixGlyphAlignmentDiagnostics(const lv_font_t* font);

// Displays a short, on-device glyph gallery for manual visual validation.
void showStixGlyphGallery(uint32_t holdMs);

} // namespace ui
