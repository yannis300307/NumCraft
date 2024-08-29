#include "world.hpp"
#include <malloc.h>

bool World::load_chunks_around(int x, int y, int z)
{
    if (loaded_chunks == NULL || available_chunk_slots == NULL)
        return false;

    // Calculate the center of the loading area in chunk space
    int chunk_pos_center_x = x / CHUNK_SIZE;
    int chunk_pos_center_y = y / CHUNK_SIZE;
    int chunk_pos_center_z = z / CHUNK_SIZE;

    // For all chunk in the view distance, we check if it is already loaded
    if (already_loaded)
    {
        for (unsigned int i = 0; i < loaded_chunk_count; i++)
        {
            Chunk chunk = loaded_chunks[i];

            int x = chunk.get_pos_x();
            int y = chunk.get_pos_y();
            int z = chunk.get_pos_z();

            available_chunk_slots[i] = !(chunk_pos_center_x - current_view_distance <= x && x >= chunk_pos_center_x + current_view_distance &&
                                         chunk_pos_center_y - current_view_distance <= y && y >= chunk_pos_center_y + current_view_distance &&
                                         chunk_pos_center_z - current_view_distance <= z && z >= chunk_pos_center_z + current_view_distance);
        }
    }

    int i = 0;

    // A lot of big for loops! It simply iterate throw the chunks in the view distance
    for (int chunk_x = chunk_pos_center_x - current_view_distance; chunk_x <= chunk_pos_center_x + current_view_distance; chunk_x++)
    {
        for (int chunk_y = chunk_pos_center_y - current_view_distance; chunk_y <= chunk_pos_center_y + current_view_distance; chunk_y++)
        {
            for (int chunk_z = chunk_pos_center_z - current_view_distance; chunk_z <= chunk_pos_center_z + current_view_distance; chunk_z++)
            {
                if (already_loaded)
                {
                    // We check if the chunk is not already loaded
                    bool found = false;
                    for (unsigned int j = 0; j < loaded_chunk_count; j++)
                    {
                        Chunk chunk = loaded_chunks[i];
                        if (chunk_x == chunk.get_pos_x() && chunk_y == chunk.get_pos_y() && chunk_z == chunk.get_pos_z())
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        // Check for an available slot
                        for (unsigned int j = 0; j < loaded_chunk_count; j++)
                        {
                            if (available_chunk_slots[j])
                            {
                                // Insert the new chunk
                                Chunk new_chunk = Chunk(x, y, z);
                                loaded_chunks[j] = new_chunk;
                                available_chunk_slots[j] = false;

                                break;
                            }
                        }
                    }
                } else {
                    // In the case of no chunks already loaded, we simply use i as the index of the cgunk slot
                    Chunk new_chunk = Chunk(x, y, z);
                    loaded_chunks[i] = new_chunk;
                    available_chunk_slots[i] = false;
                }
                i++;
            }
        }
    }

    return true;
}

bool World::change_view_distance(int view_distance)
{
    // (Re)allocate the memory
    if (loaded_chunks != NULL)
        free_all_chunks();
    loaded_chunk_count = view_distance * view_distance * view_distance;
    loaded_chunks = (Chunk *)malloc(loaded_chunk_count * sizeof(Chunk));

    current_view_distance = view_distance; // Update the view distance

    already_loaded = false; // Because we have free the chunks, we now have to reload all the chunks

    available_chunk_slots = (bool *)malloc(loaded_chunk_count);

    // Prevent out of memory issues
    return loaded_chunks != NULL && available_chunk_slots != NULL; // If loaded_chunks is equal to NULL, we are running out of memory!
}

void World::free_all_chunks()
{
    free(loaded_chunks);
}
