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
 * ComponentFactory.h — Central factory for spawning any circuit component.
 *
 * Uses Factory Pattern to create components by CompType enum.
 * All component metadata lives in PSRAM (allocated by caller).
 *
 * Part of: NumOS — Circuit Core Module
 */

#pragma once

#include "CircuitComponent.h"

namespace ComponentFactory {

/**
 * Create a circuit component by type with default parameters.
 * @param type   Component type enum
 * @param gridX  Grid X position (pixels, snapped)
 * @param gridY  Grid Y position (pixels, snapped)
 * @return Newly allocated component (caller owns), or nullptr if unknown type.
 */
CircuitComponent* create(CompType type, int gridX, int gridY);

/**
 * Get the human-readable short label for a component type.
 */
const char* label(CompType type);

/**
 * Get a tooltip description for a component type.
 */
const char* tooltip(CompType type);

/**
 * Returns true if the component type uses a voltage source index.
 */
bool needsVsIndex(CompType type);

/**
 * Returns the number of extra MNA nodes needed (beyond the standard 2).
 * E.g., transistors need a 3rd node; op-amps need a 3rd node.
 */
int extraNodes(CompType type);

/**
 * Returns true if component type should be stamped in wire/ground phase
 * (Union-Find merge before matrix stamping).
 */
bool isWirePhase(CompType type);

/**
 * Returns true if the component is a digital logic gate (uses digital sublayer).
 */
bool isLogicGate(CompType type);

} // namespace ComponentFactory
