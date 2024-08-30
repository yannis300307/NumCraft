#ifndef TEXTURE_READER_HPP
#define TEXTURE_READER_HPP

#define TEXTURE_W 160
#define TEXTURE_H 16

#include <stdint.h>

/*Give the color of the pixel in the texture at the give u and v coordinates.*/
void sampleTexture(const uint16_t *tex, int32_t u, int32_t v, uint16_t *color);

#endif