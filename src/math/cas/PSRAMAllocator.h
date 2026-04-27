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
 * PSRAMAllocator.h — Custom STL allocator that places containers in PSRAM.
 *
 * On ESP32-S3 with 8 MB OPI PSRAM, all CAS data structures (SymTerm vectors,
 * step logs, etc.) must reside in PSRAM to preserve the ~320 KB internal SRAM
 * for LVGL, stack, and real-time tasks.
 *
 * On native/PC builds (NATIVE_SIM), falls back to standard malloc/free.
 *
 * Usage:
 *   std::vector<SymTerm, PSRAMAllocator<SymTerm>> terms;
 *
 * Part of: NumOS CAS-Lite — Phase A (Algebraic Foundations)
 */

#pragma once

#include <cstddef>
#include <limits>
#include <new>

#ifdef ARDUINO
  #include <esp_heap_caps.h>
#endif

namespace cas {

// ════════════════════════════════════════════════════════════════════
// PSRAMAllocator — STL-compatible allocator for ESP32-S3 PSRAM
// ════════════════════════════════════════════════════════════════════

template <typename T>
class PSRAMAllocator {
public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    // Rebind support (required by some STL implementations)
    template <typename U>
    struct rebind { using other = PSRAMAllocator<U>; };

    PSRAMAllocator() noexcept = default;

    template <typename U>
    PSRAMAllocator(const PSRAMAllocator<U>&) noexcept {}

    // ── Allocate ────────────────────────────────────────────────────
    pointer allocate(size_type n) {
        if (n == 0) return nullptr;
        if (n > max_size()) throw std::bad_alloc();

        void* p = nullptr;

#ifdef ARDUINO
        // Prefer PSRAM (SPIRAM), fall back to internal if unavailable
        p = heap_caps_malloc(n * sizeof(T),
                             MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (!p) {
            // Fallback to any available memory
            p = heap_caps_malloc(n * sizeof(T), MALLOC_CAP_8BIT);
        }
#else
        p = std::malloc(n * sizeof(T));
#endif

        if (!p) throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    // ── Deallocate ──────────────────────────────────────────────────
    void deallocate(pointer p, size_type) noexcept {
        if (!p) return;
#ifdef ARDUINO
        heap_caps_free(p);
#else
        std::free(p);
#endif
    }

    // ── Max allocatable elements ────────────────────────────────────
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    // ── Equality (all instances are interchangeable) ────────────────
    template <typename U>
    bool operator==(const PSRAMAllocator<U>&) const noexcept { return true; }

    template <typename U>
    bool operator!=(const PSRAMAllocator<U>&) const noexcept { return false; }
};

} // namespace cas
