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
 * PowerSystems.h — Battery and power source models.
 *
 * All batteries are modeled as ideal voltage sources with
 * small internal resistance for realistic behavior.
 *
 * Part of: NumOS — Circuit Core Module
 */

#pragma once

#include "CircuitComponent.h"

// ── Battery Base Class ──────────────────────────────────────────────────────

class Battery : public CircuitComponent {
public:
    Battery(CompType type, int gridX, int gridY, float volts, float internalR);

    void stampMatrix(MnaMatrix& mna) override;
    void draw(lv_layer_t* layer, int offsetX, int offsetY) override;

    float voltage()    const { return _voltage; }
    int   vsIndex()    const { return _vsIndex; }
    void  setVsIndex(int idx) { _vsIndex = idx; }

protected:
    float _voltage;
    float _internalR;
    int   _vsIndex;

    /** Shared battery drawing with voltage label */
    void drawBattery(lv_layer_t* layer, int cx, int cy, const char* label);
};

// ── AA Battery (1.5V) ──────────────────────────────────────────────────────

class BatteryAA : public Battery {
public:
    BatteryAA(int gridX, int gridY);
    void draw(lv_layer_t* layer, int offsetX, int offsetY) override;
};

// ── Coin Cell Battery (3V, CR2032) ─────────────────────────────────────────

class BatteryCoin : public Battery {
public:
    BatteryCoin(int gridX, int gridY);
    void draw(lv_layer_t* layer, int offsetX, int offsetY) override;
};

// ── 9V Battery ─────────────────────────────────────────────────────────────

class Battery9V : public Battery {
public:
    Battery9V(int gridX, int gridY);
    void draw(lv_layer_t* layer, int offsetX, int offsetY) override;
};
