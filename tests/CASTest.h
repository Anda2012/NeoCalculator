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
 * CASTest.h — Header for CAS-Lite Phase A unit tests.
 *
 * Call cas::runCASTests() from setup() after Serial is ready.
 */

#pragma once

namespace cas {

/// Run all CAS-Lite Phase A unit tests, printing results to Serial.
void runCASTests();

} // namespace cas
