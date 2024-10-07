#include "world.hpp"
#include <malloc.h>
#include <algorithm>

bool World::load_chunks_around(int x, int y, int z)
{
    // Calculate the center of the loading area in chunk space
    int chunk_pos_center_x = x / CHUNK_SIZE;
    int chunk_pos_center_y = y / CHUNK_SIZE;
    int chunk_pos_center_z = z / CHUNK_SIZE;

    pos3D_list in_distance;

    // A lot of big for loops! It simply iterate throw the chunks in the view distance
    // List the pos of chunks in render distance
    for (int chunk_x = chunk_pos_center_x - current_view_distance; chunk_x <= chunk_pos_center_x + current_view_distance; chunk_x++)
    {
        for (int chunk_y = chunk_pos_center_y - current_view_distance; chunk_y <= chunk_pos_center_y + current_view_distance; chunk_y++)
        {
            for (int chunk_z = chunk_pos_center_z - current_view_distance; chunk_z <= chunk_pos_center_z + current_view_distance; chunk_z++)
            {
                pos3D chunk_pos = {chunk_x, chunk_y, chunk_z};

                push_back(in_distance, chunk_pos);
            }
        }
    }

    pos3D_list_iterator it = {&loaded_chunks_pos};
    for (int i = 0; i < loaded_chunks_pos.size; i++)
    {
        pos3D pos = iterator_next(it);

        if (!contains(&in_distance, pos)) {
            pop(&loaded_chunks, i);
            i--;
        }
    }

    pos3D_list_iterator it = {&in_distance};
    for (int i = 0; i < in_distance.size; i++) {
        pos3D pos = iterator_next(it);
        if (!contains(&loaded_chunks_pos, pos)) {
            Chunk new_chunck = Chunk(pos.x, pos.y, pos.z);

            push_back(&loaded_chunks, new_chunck);
    }

    pos3D_list old_poses = loaded_chunks_pos;
    loaded_chunk_count = in_distance;

    return true;
}

bool World::change_view_distance(int view_distance)
{
    /*// (Re)allocate the memory
    if (loaded_chunks != NULL)
        free_all_chunks();
    loaded_chunk_count = view_distance * view_distance * view_distance;
    loaded_chunks = (Chunk *)malloc(loaded_chunk_count * sizeof(Chunk));

    current_view_distance = view_distance; // Update the view distance

    already_loaded = false; // Because we have free the chunks, we now have to reload all the chunks

    available_chunk_slots = (bool *)malloc(loaded_chunk_count);

    // Prevent out of memory issues
    return loaded_chunks != NULL && available_chunk_slots != NULL; // If loaded_chunks is equal to NULL, we are running out of memory!*/
}

void World::free_all_chunks()
{
    // free(loaded_chunks);
}
