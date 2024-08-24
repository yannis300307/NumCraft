#ifndef texture_reader_hpp
#define texture_reader_hpp

#define TEXTURE_W 16
#define TEXTURE_H 16

#include "libs/zlib/zlib.h"

void sampleTexture(const uint8_t *tex, int32_t u, int32_t v, uint8_t *r, uint8_t *g, uint8_t *b);

#endif