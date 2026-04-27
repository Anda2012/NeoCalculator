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
 * SymSimplify.h — Multi-pass fixed-point algebraic simplifier for SymExpr DAG.
 *
 * Phase 3 upgrade: Full rule engine with fixed-point convergence.
 *
 * Architecture:
 *   simplify() runs a loop of up to MAX_PASSES bottom-up passes.
 *   Each pass applies all rule groups.  Termination is detected
 *   via pointer identity (hash-consing guarantee from Phase 2):
 *
 *       if (after == before)  →  fixed point reached  →  stop
 *
 * Rule groups (applied per pass, in order):
 *   G0: Normalize   — flatten nested Add/Mul, canonical sort
 *   G1: Identities  — 0+x→x, 1·x→x, 0·x→0, x^1→x, x^0→1
 *   G2: Constant fold — a+b→c, a·b→c, a^b→c (CASRational)
 *   G3: Like terms  — 2x+3x→5x via coefficient collection
 *   G4: Power rules — x^a·x^b→x^(a+b), (x^a)^b→x^(a·b)
 *   G5: Trig        — sin²+cos²→1, sin(-x)→-sin(x), tan→sin/cos
 *   G6: Log/Exp     — ln(e^x)→x, e^(ln x)→x, ln(a)+ln(b)→ln(a·b)
 *   G7: Cancel      — x-x→0 (via G3), x/x→1 (via G4)
 *   G8: Distribute  — a(b+c)→ab+ac (simple products only)
 *
 * Invariants:
 *   · Input is NEVER mutated (pure-functional, Phase 2 contract)
 *   · All returned nodes are cons'd (unique via ConsTable)
 *   · Weight can only stay equal or decrease (no unbounded expansion)
 *   · MAX_PASSES = 10 provides an absolute safety net
 *
 * Part of: NumOS CAS — Phase 3 (Fixed-Point Simplifier)
 */

#pragma once

#include "SymExpr.h"
#include "SymExprArena.h"

namespace cas {

class SymSimplify {
public:
    /// Maximum simplification passes before forced stop.
    /// Acts as a safety net against infinite rewrite loops in
    /// edge-case expressions (e.g. oscillating rule interactions).
    static constexpr int MAX_PASSES = 10;

    /// Simplify `expr` via multi-pass fixed-point loop.
    /// Returns a new (or same) arena-allocated, cons'd expression.
    static SymExpr* simplify(SymExpr* expr, SymExprArena& arena);

    /// Single bottom-up recursive pass (atomic mode for educational steps).
    /// Returns the same pointer if no changes were made (pointer identity).
    static SymExpr* simplifyPass(SymExpr* expr, SymExprArena& arena);

private:

    // ── Helpers ─────────────────────────────────────────────────────
    static bool isZero(const SymExpr* e);
    static bool isOne(const SymExpr* e);
    static bool isMinusOne(const SymExpr* e);
    static bool isConstNum(const SymExpr* e);
    static bool isPureNum(const SymExpr* e);  // Num with isPureRational
    static double numVal(const SymExpr* e);
    static CASRational getNumCoeff(const SymExpr* e);

    // ── Coefficient / Base extraction for like-term collection ──────
    struct CoeffBase {
        CASRational coeff;
        SymExpr*    base;   // nullptr → pure constant term
    };
    static CoeffBase extractCoeffAndBase(const SymExpr* term);

    // ── Base / Exponent extraction for power merging ────────────────
    struct BaseExp {
        SymExpr* base;
        SymExpr* exp;
    };
    static BaseExp extractBaseAndExp(const SymExpr* factor, SymExprArena& arena);

    // ── Per-type simplifiers ────────────────────────────────────────
    static SymExpr* simplifyNeg(SymNeg* n, SymExprArena& arena);
    static SymExpr* simplifyAdd(SymAdd* a, SymExprArena& arena);
    static SymExpr* simplifyMul(SymMul* m, SymExprArena& arena);
    static SymExpr* simplifyPow(SymPow* p, SymExprArena& arena);
    static SymExpr* simplifyFunc(SymFunc* f, SymExprArena& arena);

    // ── Advanced rule helpers ───────────────────────────────────────
    static SymExpr* collectLikeTerms(SymExpr** terms, uint16_t& count,
                                     SymExprArena& arena);
    static SymExpr* mergePowerBases(SymExpr** factors, uint16_t& count,
                                    SymExprArena& arena);
    static bool     detectPythagorean(SymExpr** terms, uint16_t& count,
                                      SymExprArena& arena);
    static bool     mergeLnTerms(SymExpr** terms, uint16_t& count,
                                 SymExprArena& arena);
    static SymExpr* tryDistribute(SymMul* m, SymExprArena& arena);

    // ── Pattern helpers ─────────────────────────────────────────────
    static bool isFuncOfKind(const SymExpr* e, SymFuncKind kind);
    static bool isSquareOfFunc(const SymExpr* e, SymFuncKind kind, SymExpr*& outArg);
    static SymExpr* makeCoeffTerm(const CASRational& coeff, SymExpr* base,
                                  SymExprArena& arena);
};

} // namespace cas
