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

#ifndef GIAC_BRIDGE_H
#define GIAC_BRIDGE_H

#ifdef ARDUINO
#include <Arduino.h>
#include "giac/GiacBridge.h"
#endif
#include <string>

class GiacBridge {
public:
    // Compatibility adapter for app-side access.
    static bool begin() { return true; }
    
    // Envía una expresión y devuelve el resultado en texto.
    static std::string evaluate(const std::string& expression) {
#ifdef ARDUINO
        String in(expression.c_str());
        String out = solveWithGiac(in);
        return std::string(out.c_str());
#else
        (void)expression;
        return std::string("Error: Giac available only on Arduino target");
#endif
    }
};

#endif