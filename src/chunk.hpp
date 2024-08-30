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

    // Chunk space

    int x_pos;
    int y_pos;
    int z_pos;

public:
    Chunk(int x, int y, int z);

    /*Set the block type at the given coordinates.*/
    bool set_block(uint32_t x, uint32_t y, uint32_t z, block_t type);

    /*Give the block type of the block at the given coordinates.*/
    block_t get_block(uint32_t x, uint32_t y, uint32_t z);

    /* Give the x coordiante of the chunk in chunk space*/
    int get_pos_x() { return x_pos; }
    /* Give the y coordiante of the chunk in chunk space*/
    int get_pos_y() { return y_pos; }
    /* Give the z coordiante of the chunk in chunk space*/
    int get_pos_z() { return z_pos; }
};

#endif