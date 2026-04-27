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
 * Parser.h
 * Shunting-yard based parser producing RPN from tokens.
 *
 * Precisión:
 *  - Usa los tokens generados por Tokenizer (doble precisión en números).
 *  - Gestiona precedencia: ^ > * / > + - (y soporte futuro para unarios).
 */

#pragma once

#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include "hal/ArduinoCompat.h"
#endif
#include <vector>
#include "Tokenizer.h"

struct ParseResult {
    bool ok = false;
    String errorMessage;
    std::vector<Token> outputRPN; // tokens en notación polaca inversa
};

class Parser {
public:
    Parser() = default;

    // Convierte una lista de tokens en RPN usando shunting-yard.
    ParseResult toRPN(const std::vector<Token> &tokens);

private:
    int precedence(const Token &tok) const;
    bool isRightAssociative(const Token &tok) const;
    bool isOperatorToken(const Token &tok) const;
};

