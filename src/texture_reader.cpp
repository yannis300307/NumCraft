#include "texture_reader.hpp"


void sampleTexture(const uint8_t *tex, int32_t u, int32_t v, uint8_t *r, uint8_t *g, uint8_t *b)
{
  u = u % TEXTURE_W;
  v = v % TEXTURE_H;

  const uint8_t *t = tex + (v * TEXTURE_W + u) * 3;

  *r = *t;
  t++;
  *g = *t;
  t++;
  *b = *t;
}