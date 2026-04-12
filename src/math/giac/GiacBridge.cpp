#include <sstream>
#include <exception>

#include "config.h"
#include "gen.h"
#include "global.h"
#include "prog.h"
#include "subst.h"
#include "sym2poly.h"

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
    // This Giac snapshot does not expose symbolic_mode(...); keep symbolic behavior
    // via exact evaluation level and approx_mode(false).
    giac::language(0, &global_context);
    giac::check_browser_functions();
    giac::lexer_localization(0, &global_context);
    giac::cas_setup(giac::makevecteur(0, 0, 0, 1, 0), &global_context);
    initialized = true;
  }
}

String solveWithGiac(String expr) {
  try {
    initGiac();
    std::string std_expr = expr.c_str();
    giac::gen g(std_expr, &global_context);
    g = giac::eval(g, giac::eval_level(&global_context), &global_context);
    return String(g.print(&global_context).c_str());
  } catch (const std::exception& e) {
    return String("Error: ") + e.what();
  } catch (...) {
    return String("Error: Math exception");
  }
}
