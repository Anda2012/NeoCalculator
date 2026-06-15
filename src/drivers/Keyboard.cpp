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
 * src/drivers/Keyboard.cpp
 * Legacy 6x8 keypad scanner.
 */

#include "Keyboard.h"

const KeyCode Keyboard::_map[Keyboard::ROWS][Keyboard::COLS] = {
    { KeyCode::SHIFT,  KeyCode::ALPHA,   KeyCode::MODE,     KeyCode::SETUP,
      KeyCode::F1,      KeyCode::F2,      KeyCode::F3,       KeyCode::F4 },

    { KeyCode::ON,     KeyCode::AC,       KeyCode::DEL,      KeyCode::FREE_EQ,
      KeyCode::LEFT,   KeyCode::UP,       KeyCode::DOWN,     KeyCode::RIGHT },

    { KeyCode::VAR_X,  KeyCode::VAR_Y,    KeyCode::TABLE,    KeyCode::GRAPH,
      KeyCode::ZOOM,   KeyCode::TRACE,    KeyCode::SHOW_STEPS, KeyCode::SOLVE },

    { KeyCode::NUM_7,  KeyCode::NUM_8,    KeyCode::NUM_9,    KeyCode::LPAREN,
      KeyCode::RPAREN, KeyCode::DIV,      KeyCode::POW,      KeyCode::SQRT },

    { KeyCode::NUM_4,  KeyCode::NUM_5,    KeyCode::NUM_6,    KeyCode::MUL,
      KeyCode::SUB,    KeyCode::SIN,      KeyCode::COS,      KeyCode::TAN },

    { KeyCode::NUM_1,  KeyCode::NUM_2,    KeyCode::NUM_3,    KeyCode::ADD,
      KeyCode::NEG,    KeyCode::NUM_0,    KeyCode::DOT,      KeyCode::ENTER },
};

void Keyboard::begin() {
    for (int r = 0; r < ROWS; ++r) {
        pinMode(_rowPins[r], INPUT_PULLUP);
    }

    for (int c = 0; c < COLS; ++c) {
        pinMode(_colPins[c], OUTPUT);
        digitalWrite(_colPins[c], HIGH);
    }

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            _rawState[r][c] = false;
            _debState[r][c] = false;
            _debTimer[r][c] = 0;
            _arTimer[r][c] = 0;
        }
    }

    _qHead = 0;
    _qTail = 0;
    _lastScanMs = millis();
}

void Keyboard::update() {
    uint32_t now = millis();
    if (now - _lastScanMs < SCAN_INTERVAL_MS) return;
    _lastScanMs = now;
    doScan();
}

bool Keyboard::pollEvent(KeyEvent& outEvent) {
    if (_qHead == _qTail) return false;
    outEvent = _queue[_qHead];
    _qHead = (_qHead + 1) & (QUEUE_SIZE - 1);
    return true;
}

void Keyboard::doScan() {
    uint32_t now = millis();

    for (int c = 0; c < COLS; ++c) {
        digitalWrite(_colPins[c], LOW);
        delayMicroseconds(10);

        for (int r = 0; r < ROWS; ++r) {
            bool rawNow = (digitalRead(_rowPins[r]) == LOW);

            if (rawNow != _rawState[r][c]) {
                _rawState[r][c] = rawNow;
                _debTimer[r][c] = now;
            } else if ((now - _debTimer[r][c]) >= DEBOUNCE_MS) {
                if (rawNow != _debState[r][c]) {
                    _debState[r][c] = rawNow;
                    KeyCode kc = _map[r][c];
                    if (kc != KeyCode::NONE) {
                        pushEvent({ kc, rawNow ? KeyAction::PRESS : KeyAction::RELEASE, r, c });
                        if (rawNow) {
                            _arTimer[r][c] = now;
                        }
                    }
                }
            }

            if (_debState[r][c] && _rawState[r][c]) {
                uint32_t elapsed = now - _arTimer[r][c];
                uint32_t threshold = (_arTimer[r][c] == _debTimer[r][c])
                                     ? AUTOREPEAT_DELAY_MS
                                     : AUTOREPEAT_RATE_MS;
                if (elapsed >= threshold) {
                    KeyCode kc = _map[r][c];
                    if (kc != KeyCode::NONE) {
                        pushEvent({ kc, KeyAction::REPEAT, r, c });
                    }
                    _arTimer[r][c] = now;
                }
            }
        }

        digitalWrite(_colPins[c], HIGH);
    }
}

void Keyboard::pushEvent(const KeyEvent& ev) {
    int nextTail = (_qTail + 1) & (QUEUE_SIZE - 1);
    if (nextTail == _qHead) return;
    _queue[_qTail] = ev;
    _qTail = nextTail;
}
