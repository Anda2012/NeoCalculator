#include <sstream>
#include <exception>
#include <iostream>
#include <cctype>

#include "config.h"
#include "gen.h"
#include "global.h"
#include "prog.h"
#include "subst.h"
#include "sym2poly.h"
#include "usual.h"
#include "derive.h"
#include "desolve.h"
#include "series.h"

#include "math/giac/GiacBridge.h"

using namespace giac;

namespace giac {
  void check_browser_functions();
  void lexer_localization(int lang, const context * contextptr);
}

static giac::context global_context;

static void initGiac() {
  static bool initialized = false;
  if (!initialized) {
    giac::xcas_mode(0, &global_context);
    giac::approx_mode(false, &global_context);
    giac::complex_mode(false, &global_context);
    giac::withsqrt(true, &global_context);
    giac::eval_level(&global_context) = 1;
    // Enable step-by-step infolevel for debugging/capture
    giac::step_infolevel(&global_context) = 1;
    // This Giac snapshot does not expose symbolic_mode(...); keep symbolic behavior
    // via exact evaluation level and approx_mode(false).
    giac::language(0, &global_context);
    giac::check_browser_functions();
    giac::lexer_localization(0, &global_context);
    giac::cas_setup(giac::makevecteur(0, 0, 0, 1, 0), &global_context);
    initialized = true;
  }
}

static void normalizeInfinityTokens(std::string &s) {
  auto replace_token = [&](const std::string &token, const std::string &replacement, bool skip_if_signed_left) {
    size_t pos = 0;
    while (true) {
      pos = s.find(token, pos);
      if (pos == std::string::npos) break;
      bool left_ok = (pos == 0) || (!std::isalnum((unsigned char)s[pos-1]) && s[pos-1] != '_');
      size_t after = pos + token.size();
      bool right_ok = (after >= s.size()) || (!std::isalnum((unsigned char)s[after]) && s[after] != '_');
      bool signed_left = (pos > 0) && (s[pos-1] == '+' || s[pos-1] == '-');
      if (left_ok && right_ok && !(skip_if_signed_left && signed_left)) {
        s.replace(pos, token.size(), replacement);
        pos += replacement.size();
      } else {
        pos += token.size();
      }
    }
  };

  // Explicit positive aliases
  replace_token("+oo", "+infinity", false);
  replace_token("+infty", "+infinity", false);
  // Unsuffixed aliases map to positive infinity; preserve -infinity when explicit
  replace_token("oo", "+infinity", true);
  replace_token("infty", "+infinity", true);
  replace_token("infinity", "+infinity", true);
}

static std::string trimCopy(const std::string &s) {
  size_t begin = 0;
  while (begin < s.size() && std::isspace((unsigned char)s[begin])) ++begin;
  size_t end = s.size();
  while (end > begin && std::isspace((unsigned char)s[end - 1])) --end;
  return s.substr(begin, end - begin);
}

static bool isIdentifier(const std::string &s) {
  if (s.empty()) return false;
  if (!(std::isalpha((unsigned char)s[0]) || s[0] == '_')) return false;
  for (size_t i = 1; i < s.size(); ++i) {
    unsigned char c = (unsigned char)s[i];
    if (!(std::isalnum(c) || c == '_')) return false;
  }
  return true;
}

static void replaceIdentifierToken(std::string &s, const std::string &token, const std::string &replacement) {
  size_t pos = 0;
  while (true) {
    pos = s.find(token, pos);
    if (pos == std::string::npos) break;
    bool left_ok = (pos == 0) || (!std::isalnum((unsigned char)s[pos - 1]) && s[pos - 1] != '_');
    size_t after = pos + token.size();
    bool right_ok = (after >= s.size()) || (!std::isalnum((unsigned char)s[after]) && s[after] != '_');
    if (left_ok && right_ok) {
      s.replace(pos, token.size(), replacement);
      pos += replacement.size();
    } else {
      pos += token.size();
    }
  }
}

static size_t findTopLevelChar(const std::string &s, char target) {
  int depth = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c == '(' || c == '[' || c == '{') ++depth;
    else if ((c == ')' || c == ']' || c == '}') && depth > 0) --depth;
    else if (c == target && depth == 0) return i;
  }
  return std::string::npos;
}

static void normalizeDesolvePrimeNotation(std::string &s) {
  if (s.rfind("desolve(", 0) != 0) return;
  size_t open = s.find('(');
  size_t close = s.rfind(')');
  if (open == std::string::npos || close == std::string::npos || close <= open) return;

  std::string body = s.substr(open + 1, close - open - 1);
  size_t comma = findTopLevelChar(body, ',');
  if (comma == std::string::npos) return;

  std::string eq = trimCopy(body.substr(0, comma));
  std::string y = trimCopy(body.substr(comma + 1));
  if (!isIdentifier(y)) return;

  size_t equal = findTopLevelChar(eq, '=');
  if (equal == std::string::npos) return;
  std::string left = trimCopy(eq.substr(0, equal));
  std::string right = trimCopy(eq.substr(equal + 1));

  if (left != (y + "'")) return;

  replaceIdentifierToken(right, y, y + "(x)");
  s = "desolve(diff(" + y + "(x),x)=" + right + "," + y + "(x))";
}

static bool extractDesolvePrimeCall(const std::string &s, std::string &y, std::string &rhs) {
  if (s.rfind("desolve(", 0) != 0) return false;
  size_t open = s.find('(');
  size_t close = s.rfind(')');
  if (open == std::string::npos || close == std::string::npos || close <= open) return false;

  std::string body = s.substr(open + 1, close - open - 1);
  size_t comma = findTopLevelChar(body, ',');
  if (comma == std::string::npos) return false;

  std::string eq = trimCopy(body.substr(0, comma));
  y = trimCopy(body.substr(comma + 1));
  if (!isIdentifier(y)) return false;

  size_t equal = findTopLevelChar(eq, '=');
  if (equal == std::string::npos) return false;
  std::string left = trimCopy(eq.substr(0, equal));
  rhs = trimCopy(eq.substr(equal + 1));
  return left == (y + "'");
}

static bool startsWith(const std::string &s, const char *prefix) {
  const size_t n = std::char_traits<char>::length(prefix);
  return s.size() >= n && s.compare(0, n, prefix) == 0;
}

static bool splitTopLevelArgs(const std::string &s, std::vector<std::string> &args) {
  args.clear();
  int depth = 0;
  size_t start = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c == '(' || c == '[' || c == '{') ++depth;
    else if ((c == ')' || c == ']' || c == '}') && depth > 0) --depth;
    else if (c == ',' && depth == 0) {
      args.push_back(trimCopy(s.substr(start, i - start)));
      start = i + 1;
    }
  }
  args.push_back(trimCopy(s.substr(start)));
  return !args.empty();
}

static bool splitTopLevelPower(const std::string &s, std::string &base, std::string &expo) {
  int depth = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c == '(' || c == '[' || c == '{') ++depth;
    else if ((c == ')' || c == ']' || c == '}') && depth > 0) --depth;
    else if (c == '^' && depth == 0) {
      base = trimCopy(s.substr(0, i));
      expo = trimCopy(s.substr(i + 1));
      return !base.empty() && !expo.empty();
    }
  }
  return false;
}

static bool tryFixPowOneToInfLimit(const std::string &expr, giac::gen &fixed) {
  if (!startsWith(expr, "limit(")) return false;
  size_t open = expr.find('(');
  size_t close = expr.rfind(')');
  if (open == std::string::npos || close == std::string::npos || close <= open) return false;

  std::vector<std::string> args;
  if (!splitTopLevelArgs(expr.substr(open + 1, close - open - 1), args)) return false;
  if (args.size() < 3) return false;

  const std::string &exprArg = args[0];
  const std::string &varArg = args[1];
  const std::string &limArg = args[2];
  if (!isIdentifier(varArg)) return false;

  giac::identificateur x(varArg);
  giac::gen limPoint(limArg, &global_context);
  giac::gen base;
  giac::gen expo;

  giac::gen e(exprArg, &global_context);
  if (e.type == _SYMB && e._SYMBptr->sommet == at_pow && e._SYMBptr->feuille.type == _VECT && e._SYMBptr->feuille._VECTptr->size() == 2) {
    base = e._SYMBptr->feuille._VECTptr->front();
    expo = e._SYMBptr->feuille._VECTptr->back();
  } else {
    std::string baseStr;
    std::string expoStr;
    if (!splitTopLevelPower(exprArg, baseStr, expoStr)) return false;
    base = giac::gen(baseStr, &global_context);
    expo = giac::gen(expoStr, &global_context);
  }

  giac::gen lbase = giac::limit(base, x, limPoint, 0, &global_context);
  giac::gen lexpo = giac::limit(expo, x, limPoint, 0, &global_context);
  if (!is_inf(lexpo)) return false;
  giac::gen delta = giac::recursive_normal(lbase - plus_one, &global_context);
  if (!(lbase == plus_one || is_zero(delta, &global_context))) return false;

  // If (base-1)*expo simplifies to a variable-free constant, use it directly.
  giac::gen simplified = giac::recursive_normal((base - plus_one) * expo, &global_context);
  if (!contains(giac::lidnt(simplified), x)) {
    fixed = exp(simplified, &global_context);
    return true;
  }

  giac::gen t = giac::limit(expo * (base - plus_one), x, limPoint, 0, &global_context);
  if (is_undef(t)) {
    t = giac::limit(expo * ln(base, &global_context), x, limPoint, 0, &global_context);
  }
  if (is_undef(t)) return false;

  fixed = exp(t, &global_context);
  return true;
}

static bool isSquareNumericMatrix(const giac::gen &g) {
  if (g.type != _VECT || !g._VECTptr || g._VECTptr->empty()) return false;
  const size_t n = g._VECTptr->size();
  for (size_t i = 0; i < n; ++i) {
    const giac::gen &row = (*g._VECTptr)[i];
    if (row.type != _VECT || !row._VECTptr || row._VECTptr->size() != n) return false;
  }
  return true;
}

static giac::gen diagonalToVector(const giac::gen &g) {
  if (!isSquareNumericMatrix(g)) return g;
  const size_t n = g._VECTptr->size();
  giac::vecteur diag;
  diag.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    const giac::vecteur &row = *(*g._VECTptr)[i]._VECTptr;
    for (size_t j = 0; j < n; ++j) {
      if (i != j && !is_zero(row[j], &global_context)) return g;
    }
    diag.push_back(row[i]);
  }
  return giac::gen(diag, _VECT);
}

String solveWithGiac(String expr) {
  try {
    initGiac();
    std::string std_expr = expr.c_str();

    // Serial commands may come prefixed with ':'; strip it for semantic parsing.
    std_expr = trimCopy(std_expr);
    if (!std_expr.empty() && std_expr[0] == ':') {
      std_expr = trimCopy(std_expr.substr(1));
    }

    // Normalize user-friendly infinity tokens (oo, infty, +infinity, etc.)
    normalizeInfinityTokens(std_expr);
    // Extract common ODE shorthand to avoid parser ambiguity on y'.
    std::string desolve_y;
    std::string desolve_rhs;
    bool has_desolve_prime = extractDesolvePrimeCall(std_expr, desolve_y, desolve_rhs);
    if (!has_desolve_prime) {
      // Fallback textual normalization for other forms.
      normalizeDesolvePrimeNotation(std_expr);
    }

    std::ostringstream step_buf;
    giac::gen g;
    {
      // Capture textual traces from cout/cerr and Giac logptr.
      std::streambuf* old_cout = std::cout.rdbuf(step_buf.rdbuf());
      std::streambuf* old_cerr = std::cerr.rdbuf(step_buf.rdbuf());
      std::ostream *old_log = giac::logptr(&global_context);
      giac::logptr(&step_buf, &global_context);

      if (has_desolve_prime) {
        giac::gen x{giac::identificateur("x")};
        giac::gen y{giac::identificateur(desolve_y)};
        giac::gen rhs(desolve_rhs, &global_context);
        int ordre = 0;
        giac::vecteur parameters;
        giac::gen ode = symb_equal(symb_derive(y, x), rhs);
        g = giac::desolve(ode, x, y, ordre, parameters, &global_context);
        g = giac::eval(g, giac::eval_level(&global_context), &global_context);
      } else {
        g = giac::gen(std_expr, &global_context);
        g = giac::eval(g, giac::eval_level(&global_context), &global_context);
      }

      // Fallback for 1^infinity indeterminate limits that may simplify too early.
      if (startsWith(std_expr, "limit(")) {
        giac::gen fixed;
        if (tryFixPowOneToInfLimit(std_expr, fixed)) {
          g = fixed;
        }
      }

      // Canonicalize egvl diagonal-matrix output to eigenvalue list.
      if (startsWith(std_expr, "egvl(")) {
        g = diagonalToVector(g);
      }

      giac::logptr(old_log, &global_context);
      std::cout.rdbuf(old_cout);
      std::cerr.rdbuf(old_cerr);
    }

    std::string result = g.print(&global_context);
    std::string steps = step_buf.str();
    if (!steps.empty()) {
      result += "\n[STEP_OUTPUT]\n";
      result += steps;
    }
    return String(result.c_str());
  } catch (const std::exception& e) {
    return String("Error: ") + e.what();
  } catch (...) {
    return String("Error: Math exception");
  }
}
