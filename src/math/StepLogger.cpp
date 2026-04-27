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
 * StepLogger.cpp
 */

#include "StepLogger.h"

void StepLogger::clear() {
    _steps.clear();
}

void StepLogger::addStep(const String &desc, const String &state) {
    Step s;
    s.description = desc;
    s.equationState = state;
    _steps.push_back(s);
}

