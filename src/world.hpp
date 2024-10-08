#ifndef WORLD_HPP
#define WORLD_HPP

#include "chunk.hpp"
#include <malloc.h>
#include "list.hpp"

struct pos3D
{
    int x;
    int y;
    int z;
};

static inline bool operator==(pos3D a, pos3D b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

LIST_CELL(Chunk);
LIST_CELL(pos3D);
LIST_CONTAINS(pos3D);

class World
{
private:
    Chunk_list loaded_chunks;
    pos3D_list loaded_chunks_pos;
    unsigned int loaded_chunk_count;
    int current_view_distance = 0;
    bool already_loaded = false;

public:
    /*Unload chunks that are not in the view distance and load chunks that are now in the view distance.*/
    bool load_chunks_around(int x, int y, int z);

    /*Change the view distance and realocate chunks memory. You will have to reload chunks after calling this function.*/
    bool change_view_distance(int view_distance);

    /*Free the loaded chunks to avoid memory leaks.*/
    void free_all_chunks();

    /*Return a pointer to the list of the loaded chunks.*/
    Chunk_list *get_loaded_chunks() { return &loaded_chunks; }
};

#endif