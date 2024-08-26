#ifndef texture_reader_hpp
#define texture_reader_hpp

#define TEXTURE_W 160
#define TEXTURE_H 16

#include "libs/zlib/zlib.h"
#include <stdint.h>

void sampleTexture(const uint16_t *tex, int32_t u, int32_t v, uint16_t *color);

#endif