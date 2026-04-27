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

// GiacBridge.h
#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string>
#endif

// Public API: evaluate a textual expression using Giac and return result as Arduino String.
// Input should be clean text (no trailing '\r' or '\n').
// The implementation already wraps execution in try/catch and returns "Error: ..."
// messages instead of throwing, so callers from LVGL tasks remain protected.
String solveWithGiac(String input);
