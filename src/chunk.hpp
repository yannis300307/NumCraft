#ifndef CHUNK_HPP
#define CHUNK_HPP

#define CHUNK_SIZE 8
#define CHUNK_BLOCK_COUNT 512 // 8*8*8

#include <stdint.h>


typedef unsigned char block_t;

class Chunk
{
private:
    block_t blocks[CHUNK_BLOCK_COUNT];
    unsigned int x_pos;
    unsigned int y_pos;
    unsigned int z_pos;
public:
    Chunk(unsigned int x, unsigned int y, unsigned int z);
    bool set_block(uint32_t x, uint32_t y, uint32_t z, block_t type);
    block_t get_block(uint32_t x, uint32_t y, uint32_t z);
};

#endif