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
 * StepLogger.h
 * Registro estructurado de pasos algebraicos / numéricos para Show Steps.
 */

#pragma once

#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include "hal/ArduinoCompat.h"
#endif
#include <vector>

struct Step {
    String description;   // ej. "Restar 5 a ambos lados"
    String equationState; // ej. "2x = 10" o "x1=1.5, f(x1)=..."
};

class StepLogger {
public:
    void clear();
    void addStep(const String &desc, const String &state);

    size_t size() const { return _steps.size(); }
    const Step &get(size_t idx) const { return _steps[idx]; }

private:
    std::vector<Step> _steps;
};

