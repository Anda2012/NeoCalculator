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
 * CalculusStressTest.h — Phase 6 memory stress test.
 *
 * 50 symbolic differentiations in a loop, verifying that arena.reset()
 * properly releases memory and no PSRAM leaks occur.
 *
 * Call cas::runCalculusStressTest() from setup() with -DCAS_RUN_TESTS.
 */

#pragma once

namespace cas {

/// Run 50-iteration differentiation stress test with memory tracking.
void runCalculusStressTest();

} // namespace cas
