#include "texture_reader.hpp"


void sampleTexture(const uint16_t *tex, int32_t u, int32_t v, uint16_t *color)
{
  u = u % TEXTURE_W;
  v = v % TEXTURE_H;

  *color = *(tex + (v * TEXTURE_W + u));
}