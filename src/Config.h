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
 * Config.h
 * Pinout and global configuration for the NumOS calculator.
 *
 * Hardware target: ESP32-S3 N16R8 CAM (16 MB QIO Flash + 8 MB OPI PSRAM)
 * Display       : IPS TFT 320×240 — ILI9341
 *
 * ⚠ AJUSTA los pines de TFT y teclado a tu wiring real antes de flashear.
 *   Los valores de TFT ya están en platformio.ini como -DTFT_xxx=nn.
 *   Los valores de aquí se usan en el código C++ para documentación y
 *   para módulos que NO usan TFT_eSPI directamente.
 */

#pragma once

#ifdef ARDUINO
  #include <Arduino.h>
#else
  #include "hal/ArduinoCompat.h"
#endif

// ── Pantalla TFT (ESP32-S3 CAM, bus SPI2 / FSPI) ───────────────────────────
// Deben coincidir con los -DTFT_xxx de platformio.ini.
static const int PIN_TFT_MOSI =  13;
static const int PIN_TFT_SCLK =  12;
static const int PIN_TFT_CS   =  10;
static const int PIN_TFT_DC   =   4;
static const int PIN_TFT_RST  =   5;
static const int PIN_TFT_BL   =  45;   // Backlight (PWM)

// Tamaño lógico después de la rotación 1 (landscape: 320 w × 240 h)
static const uint16_t SCREEN_WIDTH    = 320;
static const uint16_t SCREEN_HEIGHT   = 240;
static const uint8_t  SCREEN_ROTATION = 1;

// ── Teclado físico legacy 6×8 — cableado actual ─────────────────────────────
//
// Escaneo: Filas = INPUT_PULLUP (LOW = pulsada), Columnas = OUTPUT (LOW activo).
//
// Legacy 6x8 keypad wiring.
//
// Filas (INPUT_PULLUP)
static const int KBD_ROW_PINS[6] = { 1, 2, 3, 6, 7, 8 };

// Columnas (OUTPUT, LOW activo)
static const int KBD_COL_PINS[8] = { 38, 39, 40, 41, 42, 47, 48, 21 };

static const uint8_t KBD_ROWS           = 6;
static const uint8_t KBD_COLS           = 8;
static const uint8_t KBD_CONNECTED_COLS = 8;   // ← Aumentar al conectar más columnas

// ── Temporización del escaneo (compartida con ambos drivers) ────────────────
static const uint16_t KEY_SCAN_INTERVAL_MS    =  5;   // ms entre escaneos
static const uint16_t KEY_DEBOUNCE_MS         = 20;   // ms de anti-rebote
static const uint16_t KEY_AUTOREPEAT_DELAY_MS = 500;  // ms antes del autorepeat
static const uint16_t KEY_AUTOREPEAT_RATE_MS  =  80;  // ms entre REPEAT events

// ── CAS settings ────────────────────────────────────────────────────────────
extern bool setting_complex_enabled;   // true = show complex roots, false = "No real solutions"
extern int  setting_decimal_precision;  // number of decimal digits (6, 8, 10, 12)
extern bool setting_edu_steps;          // true = step-by-step educational mode for arithmetic

// ── Matriz legacy 6×8 (reservada / compatibilidad con KeyMatrix.h) ───────────
// Filas: INPUT_PULLUP.  Columnas: OUTPUT activo-LOW.
// Solo usado por la clase KeyMatrix; no conectado en el hardware actual.
static const int PIN_KEY_R0 =  1;
static const int PIN_KEY_R1 =  2;
static const int PIN_KEY_R2 =  3;
static const int PIN_KEY_R3 =  6;
static const int PIN_KEY_R4 =  7;
static const int PIN_KEY_R5 =  8;

static const int PIN_KEY_C0 = 38;
static const int PIN_KEY_C1 = 39;
static const int PIN_KEY_C2 = 40;
static const int PIN_KEY_C3 = 41;
static const int PIN_KEY_C4 = 42;
static const int PIN_KEY_C5 = 47;
static const int PIN_KEY_C6 = 48;
static const int PIN_KEY_C7 = 21;

static const uint8_t KEY_ROWS = 6;
static const uint8_t KEY_COLS = 8;

