#ifndef WORLD_HPP
#define WORLD_HPP

#include "chunk.hpp"
#include <malloc.h>

class World
{
private:
    Chunk *loaded_chunks = NULL; // Dynamically allowed with malloc
    unsigned int loaded_chunk_count;
    int current_view_distance = 0;
    bool *available_chunk_slots; // Used to optimise chunk memory allocation
    bool already_loaded = false;

public:
    /*Unload chunks that are not in the view distance and load chunks that are now in the view distance.*/
    bool load_chunks_around(int x, int y, int z);

    /*Change the view distance and realocate chunks memory. You will have to reload chunks after calling this function.*/
    bool change_view_distance(int view_distance);

    /*Free the loaded chunks to avoid memory leaks.*/
    void free_all_chunks();
};

#endif