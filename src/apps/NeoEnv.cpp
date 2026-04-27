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
 * NeoEnv.cpp — Hierarchical symbol table implementation.
 *
 * Part of: NeoCalculator / NumOS — NeoLanguage Phase 2 (Interpreter)
 */

#include "NeoEnv.h"

// ════════════════════════════════════════════════════════════════════
// lookup
// ════════════════════════════════════════════════════════════════════

const NeoValue* NeoEnv::lookup(const std::string& name) const {
    auto it = _table.find(name);
    if (it != _table.end()) return &it->second;
    if (_parent) return _parent->lookup(name);
    return nullptr;
}

NeoValue* NeoEnv::lookup(const std::string& name) {
    auto it = _table.find(name);
    if (it != _table.end()) return &it->second;
    if (_parent) return _parent->lookup(name);
    return nullptr;
}

// ════════════════════════════════════════════════════════════════════
// define — create binding in THIS scope
// ════════════════════════════════════════════════════════════════════

void NeoEnv::define(const std::string& name, NeoValue val) {
    _table[name] = std::move(val);
}

// ════════════════════════════════════════════════════════════════════
// assign — update nearest existing binding, or create here
// ════════════════════════════════════════════════════════════════════

void NeoEnv::assign(const std::string& name, NeoValue val) {
    // Walk up the chain looking for an existing binding
    NeoEnv* env = this;
    while (env) {
        auto it = env->_table.find(name);
        if (it != env->_table.end()) {
            it->second = std::move(val);
            return;
        }
        env = env->_parent;
    }
    // Not found anywhere → create in this scope
    _table[name] = std::move(val);
}
