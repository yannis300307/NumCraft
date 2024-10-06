#include "mesh_builder.hpp"
#include <malloc.h>
#include <list>
#include "list.hpp"

using namespace std;

void gen_block_mesh(int x, int y, int z, S3L_Model3D &level_model)
{
}

struct triangle
{
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    int x3;
    int y3;
    int z3;
};

LIST_CELL(triangle);

void build_chunk_mesh(Chunk *chunk, S3L_Model3D *model)
{
    triangle_list triangles_list;

    // Generate the triangles
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (chunk->get_block(x, y, z) > 0) // Check if the current block is not air
                {
                    // Check each faces

                    // front
                    if (x >= 0 && y >= 0 && z - 1 >= 0 && // Check oob
                        x < CHUNK_SIZE && y < CHUNK_SIZE && z - 1 < CHUNK_SIZE &&
                        // check if the block is air
                        chunk->get_block(x, y, z - 1) == 0)
                    {
                        push_back(&triangles_list, {x, y, z, x, y + 1, z, x + 1, y + 1, z});
                        push_back(&triangles_list, {x, y, z, x + 1, y, z, x + 1, y + 1, z});
                    }

                    // back
                    if (x >= 0 && y >= 0 && z + 1 >= 0 &&
                        x < CHUNK_SIZE && y < CHUNK_SIZE && z + 1 < CHUNK_SIZE &&
                        chunk->get_block(x, y, z + 1) == 0)
                    {
                        push_back(&triangles_list, {x + 1, y, z + 1, x, y, z + 1, x, y + 1, z + 1});
                        push_back(&triangles_list, {x + 1, y, z + 1, x + 1, y + 1, z + 1, x, y + 1, z + 1});
                    }

                    // top
                    if (x >= 0 && y + 1 >= 0 && z >= 0 &&
                        x < CHUNK_SIZE && y + 1 < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x, y + 1, z) == 0)
                    {
                        push_back(&triangles_list, {x, y + 1, z, x, y + 1, z + 1, x + 1, y + 1, z + 1});
                        push_back(&triangles_list, {x, y + 1, z, x + 1, y + 1, z, x + 1, y + 1, z + 1});
                    }

                    // bottom
                    if (x >= 0 && y - 1 >= 0 && z >= 0 &&
                        x < CHUNK_SIZE && y - 1 < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x, y - 1, z) == 0)
                    {
                        push_back(&triangles_list, {x, y, z, x, y, z + 1, x + 1, y, z + 1});
                        push_back(&triangles_list, {x, y, z, x + 1, y, z, x + 1, y, z + 1});
                    }

                    // right
                    if (x >= 0 && y >= 0 && z >= 0 &&
                        x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x, y, z) == 0)
                    {
                        push_back(&triangles_list, {x + 1, y, z, x + 1, y + 1, z, x + 1, y + 1, z + 1});
                        push_back(&triangles_list, {x + 1, y, z, x + 1, y, z + 1, x + 1, y + 1, z + 1});
                    }

                    // left
                    if (x - 1 >= 0 && y >= 0 && z >= 0 &&
                        x - 1 < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x - 1, y, z) == 0)
                    {
                        push_back(&triangles_list, {x, y, z + 1, x, y + 1, z + 1, x, y + 1, z});
                        push_back(&triangles_list, {x, y, z + 1, x, y, z, x, y + 1, z});
                    }
                }
            }
        }
    }

    int triangle_count = triangles_list.size;

    S3L_Unit *verticies = (S3L_Unit *)malloc(triangle_count * 9); // 9 int per triangle
    S3L_Index *triangles = (S3L_Index *)malloc(triangle_count);

    for (int i = 0; i < triangle_count; i++)
    {
        triangle current_triangle = pop_front(&triangles_list);

        verticies[i * 9] = current_triangle.x1;
        verticies[i * 9 + 1] = current_triangle.y1;
        verticies[i * 9 + 2] = current_triangle.z1;
        triangles[i * 3] = i * 3;

        verticies[i * 9 + 3] = current_triangle.x2;
        verticies[i * 9 + 4] = current_triangle.y2;
        verticies[i * 9 + 5] = current_triangle.z2;
        triangles[i * 3 + 1] = i * 3 + 1;

        verticies[i * 9 + 6] = current_triangle.x3;
        verticies[i * 9 + 7] = current_triangle.y3;
        verticies[i * 9 + 8] = current_triangle.z3;
        triangles[i * 3 + 2] = i * 3 + 2;
    }

    model->triangles = triangles;
    model->triangleCount = triangle_count;
    model->vertexCount = triangle_count * 9;
    model->vertices = verticies;
}