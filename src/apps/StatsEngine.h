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
 * StatsEngine.h — Statistical computation engine for NumOS.
 *
 * Processes paired data (values + frequencies) and computes:
 *   - Mean, Median, Standard Deviation, Min, Max, Sum, Count
 *
 * Pure C++ — no LVGL or Arduino dependencies.
 */

#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <cstddef>

class StatsEngine {
public:
    struct DataPoint {
        double value;
        double frequency;   // ≥ 1
    };

    void clear();
    void setData(const std::vector<DataPoint>& data);
    size_t count() const;          ///< Total frequency (Σ freq)

    double getMean() const;
    double getMedian() const;
    double getStandardDeviation() const;
    double getMin() const;
    double getMax() const;
    double getSum() const;

    const std::vector<DataPoint>& data() const { return _data; }

private:
    std::vector<DataPoint> _data;
};
