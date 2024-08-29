#include "chunk.hpp"

#define get_index(x, y, z) x + y *CHUNK_SIZE + z *CHUNK_SIZE *CHUNK_SIZE

Chunk::Chunk(unsigned int x, unsigned int y, unsigned int z)
{
    x_pos = x;
    y_pos = y;
    z_pos = z;

    for (int i = 0; i < CHUNK_BLOCK_COUNT; i++)
    {
        blocks[i] = 0;
    }
}

bool Chunk::set_block(uint32_t x, uint32_t y, uint32_t z, block_t type)
{
    if ((x >= CHUNK_SIZE) || (y >= CHUNK_SIZE) || (z >= CHUNK_SIZE))
        return false;
    blocks[get_index(x, y, z)] = type;
    return true;
}

block_t Chunk::get_block(uint32_t x, uint32_t y, uint32_t z) {
    return blocks[get_index(x, y, z)];
}