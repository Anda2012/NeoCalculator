#pragma once

// Minimal TI CSDK compatibility declarations for non-TI targets.
#ifndef COLOR_BLACK
#define COLOR_BLACK 0
#endif

#ifndef COLOR_WHITE
#define COLOR_WHITE 65535
#endif

#ifndef LCD_WIDTH_PX
#define LCD_WIDTH_PX 320
#endif

#ifndef LCD_HEIGHT_PX
#define LCD_HEIGHT_PX 240
#endif

// Some sources alias PrintMini to Printmini.
static inline int Printmini(int, int, const char *, int, int) { return 0; }

// Minimal RTC/time stubs used by Giac legacy code paths.
static int rtc_Minutes = 0;
static int rtc_Seconds = 0;
static inline void set_time(int, int) {}
static inline unsigned long millis() { return 0; }
