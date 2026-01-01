#include "screen.h"
#include <stdint.h>

extern const uint8_t font_petme128_8x8[][8];
;

void draw_text(UBYTE *image, uint8_t start_x, uint8_t start_y,
               const char *text);
