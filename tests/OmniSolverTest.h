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
 * OmniSolverTest.h — Header for Phase 4 unit tests.
 *
 * Covers OmniSolver (classification + dispatch), HybridNewton
 * (Newton-Raphson with exact symbolic Jacobian), and the
 * algebraic inverse path.
 *
 * Call cas::runOmniSolverTests() from setup() after Serial is ready.
 * Requires -DCAS_RUN_TESTS build flag.
 */

#pragma once

namespace cas {

/// Run all Phase 4 unit tests (OmniSolver, HybridNewton, Inverse).
void runOmniSolverTests();

} // namespace cas
