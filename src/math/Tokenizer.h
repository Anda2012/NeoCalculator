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
 * Tokenizer.h
 * Lexical analyzer for calculator expressions.
 *
 * Features:
 *  - Uses double precision for numeric literals.
 *  - Accepts `.1` and normaliza a `0.1`.
 *  - Interprets aislado `.` como `0` cuando va seguido de un operador.
 *  - Inserta multiplicación implícita: 2x, 2(x+1), (x+1)(x-1).
 *  - Distingue variables/identificadores (A..Z, x, y1, etc.) y funciones (ident seguidos de `(`).
 */

#pragma once

#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include "hal/ArduinoCompat.h"
#endif
#include <vector>

enum class TokenType : uint8_t {
    End = 0,
    Number,
    Identifier,   // variable (x, A..Z, etc.)
    Function,     // function name (sin, cos, etc.)
    Operator,
    LParen,
    RParen,
    Comma,
    Equal
};

enum class OperatorKind : uint8_t {
    Plus = 0,
    Minus,
    Mul,
    Div,
    Pow
};

struct Token {
    TokenType type = TokenType::End;
    String    text;   // raw text (for identifiers/operators)
    double    value = 0.0;  // for Number
    OperatorKind op = OperatorKind::Plus; // for Operator
};

struct TokenizeResult {
    bool ok = false;
    String errorMessage;
    std::vector<Token> tokens;
};

class Tokenizer {
public:
    Tokenizer() = default;

    // Tokeniza una expresión completa. Agrega un token final de tipo End.
    TokenizeResult tokenize(const String &input);

private:
    String _input;
    int _len = 0;
    int _pos = 0;

    bool isAtEnd() const { return _pos >= _len; }
    char peek(int offset = 0) const;
    char peekNextNonSpace() const;
    char advance();
    void skipSpaces();

    bool isIdentStart(char c) const;
    bool isIdentPart(char c) const;

    bool readNumber(Token &outTok);
    bool readIdentifierOrFunction(Token &outTok);
    bool readOperatorOrSymbol(Token &outTok, String &error);

    bool isValueLike(const Token &t) const;
    bool isValueStartChar(char c) const;

    void appendImplicitMulIfNeeded(std::vector<Token> &out, const Token &next);
};

