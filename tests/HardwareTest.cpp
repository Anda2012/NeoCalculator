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
 * HardwareTest.cpp
 * 
 * Simple test to verify:
 * 1. Keypad scanning (prints Row/Col or Key Name to Serial/LCD)
 * 2. Display functionality.
 * 
 * Usage: Swap this with main.cpp or include in main logic to test.
 */

#include <Arduino.h>
#include "display/DisplayDriver.h"
#include "input/KeyMatrix.h"

// Globals for test
static DisplayDriver tftTest;
static KeyMatrix keyTest;

void setupTest() {
    Serial.begin(115200);
    
    tftTest.begin();
    tftTest.clear(TFT_BLACK);
    tftTest.setTextColor(TFT_WHITE, TFT_BLACK);
    tftTest.setTextSize(2);
    tftTest.drawText(10, 10, "HARDWARE TEST");
    tftTest.setTextSize(1);
    tftTest.drawText(10, 40, "Press any key...");
}

void loopTest() {
    keyTest.update();
    
    KeyEvent ev;
    while(keyTest.pollEvent(ev)) {
        if (ev.action == KeyAction::PRESS) {
            String msg = "Key: ";
            // Simple mapping to text manually or just code
            msg += String((int)ev.code);
            msg += " R=" + String(ev.row) + " C=" + String(ev.col);
            
            Serial.println(msg);
            
            tftTest.tft().fillRect(0, 60, 240, 20, TFT_BLACK); // Clear line
            tftTest.drawText(10, 60, msg);
        }
    }
    delay(10);
}
