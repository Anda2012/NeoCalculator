#include "main.h"

#include <map>
#include <string>

#include "gen.h"
#include "global.h"
#include "graphtheory.h"
#include "prog.h"
#include "tinymt32.h"

// Keep a global fallback for legacy includes that declare freeze in global scope.
bool freeze = false;

extern "C" int ctrl_c_interrupted(int) { return 0; }

void tinymt32_init(tinymt32_t *random, uint32_t seed) {
	if (!random) {
		return;
	}
	random->status[0] = seed;
	random->status[1] = 0x8f7011eeU;
	random->status[2] = 0xfc78ff1fU;
	random->status[3] = 0x3793fdffU;
	random->mat1 = 0x8f7011eeU;
	random->mat2 = 0xfc78ff1fU;
	random->tmat = 0x3793fdffU;
}

void tinymt32_init_by_array(tinymt32_t *random, uint32_t init_key[], int key_length) {
	uint32_t seed = 1U;
	if (init_key && key_length > 0) {
		seed = init_key[0];
	}
	tinymt32_init(random, seed);
}

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif

struct order_t;

bool freeze = false;
std::map<std::string, context *> *context_names = nullptr;

vecteur *keywords_vecteur_ptr() { return nullptr; }

#ifndef TICE
void set_abort() {}
void clear_abort() {}
#endif

void draw_line(int, int, int, int, int, unsigned short) {}
void draw_circle(int, int, int, int, bool, bool, bool, bool) {}
void draw_arc(int, int, int, int, int, double, double, GIAC_CONTEXT) {}
void draw_arc(int, int, int, int, int, double, double, bool, bool, bool, bool, GIAC_CONTEXT) {}
void draw_filled_circle(int, int, int, int, bool, bool) {}
void draw_rectangle(int, int, int, int, unsigned short) {}
void drawRectangle(int, int, int, int, unsigned short) {}

std::map<const char *, const mksa_unit *, ltstr> &unit_conversion_map() {
	static std::map<const char *, const mksa_unit *, ltstr> empty;
	return empty;
}

gen symb_unit(const gen &a, const gen &, GIAC_CONTEXT) { return a; }

bool gbasis8(const vectpoly &, order_t &, vectpoly &, environment *, bool, bool, int &, GIAC_CONTEXT, bool) {
	return false;
}

bool greduce8(const vectpoly &, const vectpoly &, order_t &, vectpoly &, environment *, GIAC_CONTEXT) {
	return false;
}

bool is_graphe(const gen &, std::string &disp_out, GIAC_CONTEXT) {
	disp_out.clear();
	return false;
}

#ifndef NO_NAMESPACE_GIAC
}
#endif
