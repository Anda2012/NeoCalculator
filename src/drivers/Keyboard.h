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
 * src/drivers/Keyboard.h
 * Legacy 6x8 keypad scanner used by the original PCB layout.
 */

#pragma once

#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include "../hal/ArduinoCompat.h"
#endif

#include "../input/KeyCodes.h"
#include "../Config.h"

class Keyboard {
public:
    Keyboard() = default;

    void begin();
    void update();
    bool pollEvent(KeyEvent& outEvent);

    static constexpr int ROWS = 6;
    static constexpr int COLS = 8;
    static constexpr int CONNECTED_COLS = COLS;

private:
    const int _rowPins[ROWS] = { PIN_KEY_R0, PIN_KEY_R1, PIN_KEY_R2, PIN_KEY_R3, PIN_KEY_R4, PIN_KEY_R5 };
    const int _colPins[COLS] = { PIN_KEY_C0, PIN_KEY_C1, PIN_KEY_C2, PIN_KEY_C3, PIN_KEY_C4, PIN_KEY_C5, PIN_KEY_C6, PIN_KEY_C7 };

    static constexpr uint16_t SCAN_INTERVAL_MS    = 5;
    static constexpr uint16_t DEBOUNCE_MS         = 20;
    static constexpr uint16_t AUTOREPEAT_DELAY_MS = 500;
    static constexpr uint16_t AUTOREPEAT_RATE_MS  = 80;

    static const KeyCode _map[ROWS][COLS];

    bool     _rawState[ROWS][COLS] {};
    bool     _debState[ROWS][COLS] {};
    uint32_t _debTimer[ROWS][COLS] {};
    uint32_t _arTimer[ROWS][COLS] {};
    uint32_t _lastScanMs = 0;

    static constexpr int QUEUE_SIZE = 16;
    KeyEvent _queue[QUEUE_SIZE];
    int      _qHead = 0;
    int      _qTail = 0;

    void doScan();
    void pushEvent(const KeyEvent& ev);
};
