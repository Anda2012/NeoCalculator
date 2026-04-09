#include <sstream>
#include <exception>

#include "config.h"
#include "gen.h"

#include "math/giac/GiacBridge.h"

using namespace giac;

String solveWithGiac(String input) {
  std::string expr = std::string(input.c_str());
  try {
    static context ct;
    gen g(expr, &ct);
    g = eval(g, 1, &ct);
    std::ostringstream oss;
    oss << g;
    std::string out = oss.str();
    return String(out.c_str());
  } catch (const std::exception &e) {
    Serial.print("[GiacBridge] exception: ");
    Serial.println(e.what());
    return String("ERROR: ") + String(e.what());
  } catch (...) {
    Serial.println("[GiacBridge] unknown exception");
    return String("ERROR: unknown");
  }
}
