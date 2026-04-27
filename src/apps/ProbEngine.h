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
 * ProbEngine.h — Probability computation engine for NumOS.
 *
 * Implements Normal Distribution:
 *   - PDF (Probability Density Function)
 *   - CDF (Cumulative Distribution Function) via Abramowitz & Stegun ERF
 *
 * Pure C++ — no LVGL or Arduino dependencies.
 */

#pragma once

#include <cmath>

class ProbEngine {
public:
    /// Normal distribution PDF: f(x) = (1/(σ√(2π))) · e^(-½((x-μ)/σ)²)
    static double normalPDF(double x, double mu, double sigma);

    /// Normal distribution CDF: P(X ≤ x) using ERF approximation
    static double normalCDF(double x, double mu, double sigma);

    /// Error function approximation (Abramowitz & Stegun, formula 7.1.26)
    static double erf_approx(double x);

    /// Generate PDF curve points for plotting
    /// Fills yOut[0..nPoints-1] with PDF values from xMin to xMax
    static void generatePDFCurve(double mu, double sigma,
                                  double xMin, double xMax,
                                  double* yOut, int nPoints);
};
