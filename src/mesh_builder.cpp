#include "mesh_builder.hpp"
#include <malloc.h>
#include <list>

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

void build_chunk_mesh(Chunk *chunk, S3L_Model3D *model)
{
    list<triangle> triangles;

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
                        triangles.push_back({x, y, z, x, y + 1, z, x + 1, y + 1, z});
                        triangles.push_back({x, y, z, x + 1, y, z, x + 1, y + 1, z});
                    }

                    // back
                    if (x >= 0 && y >= 0 && z + 1 >= 0 &&
                        x < CHUNK_SIZE && y < CHUNK_SIZE && z + 1 < CHUNK_SIZE &&
                        chunk->get_block(x, y, z + 1) == 0)
                    {
                        triangles.push_back({x + 1, y, z + 1, x, y, z + 1, x, y + 1, z + 1});
                        triangles.push_back({x + 1, y, z + 1, x + 1, y + 1, z + 1, x, y + 1, z + 1});
                    }

                    // top
                    if (x >= 0 && y + 1 >= 0 && z >= 0 &&
                        x < CHUNK_SIZE && y + 1 < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x, y + 1, z) == 0)
                    {
                        triangles.push_back({x, y + 1, z, x, y + 1, z + 1, x + 1, y + 1, z + 1});
                        triangles.push_back({x, y + 1, z, x + 1, y + 1, z, x + 1, y + 1, z + 1});
                    }

                    // bottom
                    if (x >= 0 && y - 1 >= 0 && z >= 0 &&
                        x < CHUNK_SIZE && y - 1 < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x, y - 1, z) == 0)
                    {
                        triangles.push_back({x, y, z, x, y, z + 1, x + 1, y, z + 1});
                        triangles.push_back({x, y, z, x + 1, y, z, x + 1, y, z + 1});
                    }

                    // right
                    if (x >= 0 && y >= 0 && z >= 0 &&
                        x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x, y, z) == 0)
                    {
                        triangles.push_back({x + 1, y, z, x + 1, y + 1, z, x + 1, y + 1, z + 1});
                        triangles.push_back({x + 1, y, z, x + 1, y, z + 1, x + 1, y + 1, z + 1});
                    }

                    // left
                    if (x - 1 >= 0 && y >= 0 && z >= 0 &&
                        x - 1 < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE &&
                        chunk->get_block(x - 1, y, z) == 0)
                    {
                        triangles.push_back({x, y, z + 1, x, y + 1, z + 1, x, y + 1, z});
                        triangles.push_back({x, y, z + 1, x, y, z, x, y + 1, z});
                    }
                }
            }
        }
    }

    /*S3L_Index *triangles = (S3L_Index *)malloc(triangle_count * sizeof(S3L_Index));

    model->triangles = triangles;
    model->triangleCount = triangle_count;
    model->vertices;*/
}