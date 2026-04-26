#include "MathTypography.h"

#include "../fonts/StixMathFont.h"

namespace ui {

lv_style_t style_math_primary;

static bool g_mathTypographyInited = false;

void initMathTypography() {
    if (g_mathTypographyInited) {
        return;
    }

    lv_style_init(&style_math_primary);
    lv_style_set_text_font(&style_math_primary, &stix_math_18);
    lv_style_set_text_color(&style_math_primary, lv_color_black());
    lv_style_set_text_opa(&style_math_primary, LV_OPA_COVER);

    g_mathTypographyInited = true;
}

const lv_font_t* mathPrimaryFont() {
    return &stix_math_18;
}

} // namespace ui
