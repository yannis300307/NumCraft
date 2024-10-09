#include "renderer.hpp"
#include "texture_reader.hpp"
#include "textures/tileset.h"
#include "core.hpp"

#define S3L_PIXEL_FUNCTION draw_pixel
#define S3L_RESOLUTION_X 320
#define S3L_RESOLUTION_Y 240
#define PIXEL_COUNT (S3L_RESOLUTION_X * S3L_RESOLUTION_Y)
#define Z_BUFFER_BIT_MAP (S3L_RESOLUTION_X * S3L_RESOLUTION_Y / 8)
#define S3L_PERSPECTIVE_CORRECTION 1
#define S3L_SORT 2

#include "libs/small3dlib.h"
#include <bitset>

using namespace std;

bitset<PIXEL_COUNT> z_buffer_bit_map;

// Temp
S3L_Unit cubeVertices[] = {S3L_CUBE_VERTICES(S3L_F)};
S3L_Index cubeTriangles[] = {S3L_CUBE_TRIANGLES};
S3L_Unit cubeUvs[] = {S3L_CUBE_TEXCOORDS(16)};
S3L_Index cubeuvIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

S3L_Model3D cubeModel;

S3L_Unit cubeVertices2[] = {512 + 512 / 2, 512 + -512 / 2, 512 + -512 / 2, 512 + -512 / 2, 512 + -512 / 2, 512 + -512 / 2, 512 + 512 / 2, 512 + 512 / 2, 512 + -512 / 2, 512 + -512 / 2, 512 + 512 / 2, 512 + -512 / 2, 512 + 512 / 2, 512 + -512 / 2, 512 + 512 / 2, 512 + -512 / 2, 512 + -512 / 2, 512 + 512 / 2, 512 + 512 / 2, 512 + 512 / 2, 512 + 512 / 2, 512 + -512 / 2, 512 + 512 / 2, 512 + 512 / 2};
S3L_Index cubeTriangles2[] = {S3L_CUBE_TRIANGLES};
S3L_Unit cubeUvs2[] = {S3L_CUBE_TEXCOORDS(16)};
S3L_Index cubeuvIndices2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

S3L_Model3D cubeModel2;

// Setup objects used by S3L
S3L_Model3D *models = NULL; // Array of the models in the scene dynamically allowed by malloc
int model_count = 1;
S3L_Scene scene;

// Used for caching
uint32_t previousTriangle = -1;
uint16_t last_pixel_x = S3L_RESOLUTION_X; // Must be an impossible value at initialisation
uint16_t last_pixel_y = S3L_RESOLUTION_Y; // Same
uint16_t pixels_in_row_count = 0;
eadk_color_t row_pixel_buffer[S3L_RESOLUTION_X];

S3L_Vec4 uv0, uv1, uv2;
const uint16_t *texture = tilesetTexture;
const S3L_Index *uvIndices = cubeuvIndices;
const S3L_Unit *uvs = cubeUvs;

int current_view_distance = 0;

void build_chunk_mesh(Chunk *chunk, S3L_Model3D *model);

void clear_z_buffer_bit_map()
/*Clear the the bit map z buffer.*/
{
    z_buffer_bit_map.reset();
}

void draw_pixel(S3L_PixelInfo *pixel)
/*Draw the pixel gived by the rasterizer.*/
{
    int index = pixel->x + pixel->y * S3L_RESOLUTION_X;

    // Recover informations about the uvs
    if (pixel->triangleID != previousTriangle)
    {
        S3L_getIndexedTriangleValues(pixel->triangleIndex, uvIndices, uvs, 2, &uv0, &uv1, &uv2);
        previousTriangle = pixel->triangleID;
    }

    if (z_buffer_bit_map.test(index))
        return;

    // Recover the color of the pixel
    S3L_Unit uv[2];
    uv[0] = S3L_interpolateBarycentric(uv0.x, uv1.x, uv2.x, pixel->barycentric);
    uv[1] = S3L_interpolateBarycentric(uv0.y, uv1.y, uv2.y, pixel->barycentric);
    uint16_t color;
    sampleTexture(texture, uv[0], uv[1], &color);

    // Calculate the triangle row
    uint16_t x = pixel->x;
    uint16_t y = pixel->y;
    uint16_t w = last_pixel_x - pixels_in_row_count + 1;

    // If the triangle row has changed, we push the entire line to the screen
    if (!(y == last_pixel_y && x == last_pixel_x + 1))
    {
        eadk_display_push_rect({w, last_pixel_y, pixels_in_row_count, 1}, row_pixel_buffer);
        pixels_in_row_count = 0;
    }

    last_pixel_x = x;
    last_pixel_y = y;
    row_pixel_buffer[pixels_in_row_count] = color;
    pixels_in_row_count += 1;

    // Set the z bit map buffer.
    z_buffer_bit_map.set(index);
}

bool Renderer::change_view_distance(int view_distance)
{
    // Free the memory of the old model array
    if (models != NULL)
        free(models);
    models = (S3L_Model3D *)malloc(view_distance * view_distance * view_distance * sizeof(S3L_Model3D)); // Allocate the space for the chunk models

    if (models == NULL)
        return false;

    current_view_distance = view_distance;

    model_count = view_distance * view_distance * view_distance;

    scene.modelCount = model_count;

    Chunk_list * chunks_list = core->world.get_loaded_chunks();
    Chunk_list_iterator it = {chunks_list};

    
    for (int i = 0; i < model_count; i++)
    {
        S3L_Model3D chunk_model;
        
        S3L_model3DInit(0, 0, 0, 0, &chunk_model);

        Chunk *current_chunk = iterator_next(&it);

        build_chunk_mesh(current_chunk, &chunk_model);

        continue;

        models[i] = chunk_model;
    }

    S3L_sceneInit( // Initialize the scene we'll be rendering.
            models,
            model_count,
            &scene);

    return true;
}

Renderer::Renderer()
{
    scene = S3L_Scene();

    // temp
    S3L_model3DInit(
        cubeVertices,
        S3L_CUBE_VERTEX_COUNT,
        cubeTriangles,
        S3L_CUBE_TRIANGLE_COUNT,
        &cubeModel);

    models = (S3L_Model3D *)malloc(sizeof(S3L_Model3D));
    models[0] = cubeModel;

    model_count = 1;

    S3L_sceneInit( // Initialize the scene we'll be rendering.
        models,
        model_count,
        &scene);

    scene.camera.transform.translation.z = -2 * S3L_F;
}

void fill_void_space(eadk_color_t color)
/*Fill the screen where triangles are not rendered on the last frame depending of the bit map z buffer.*/
{
    eadk_color_t line[S3L_RESOLUTION_X];
    for (uint16_t x = 0; x < S3L_RESOLUTION_X; x++)
    {
        eadk_display_pull_rect({x, 0, 1, S3L_RESOLUTION_Y}, line);
        for (uint16_t y = 0; y < S3L_RESOLUTION_Y; y++)
        {
            if (!z_buffer_bit_map[x + y * S3L_RESOLUTION_X])
            {
                line[y] = color;
            }
        }
        eadk_display_push_rect({x, 0, 1, S3L_RESOLUTION_Y}, line);
    }
}

void Renderer::move_camera(int x, int y, int z)
{
    scene.camera.transform.translation.x += x;
    scene.camera.transform.translation.y += y;
    scene.camera.transform.translation.z += z;
}

void Renderer::move_camera_forward(int step)
{

    S3L_Vec4 camDirection;

    S3L_vec4Init(&camDirection);

    S3L_rotationToDirections(scene.camera.transform.rotation, S3L_F, &camDirection, 0, 0);

    scene.camera.transform.translation.x += (camDirection.x * step) / S3L_F;
    scene.camera.transform.translation.z += (camDirection.z * step) / S3L_F;
}

void Renderer::move_camera_sideway(int step)
{

    S3L_Vec4 camDirection;

    S3L_vec4Init(&camDirection);

    S3L_rotationToDirections(scene.camera.transform.rotation, S3L_F, 0, &camDirection, 0);

    scene.camera.transform.translation.x += (camDirection.x * step) / S3L_F;
    scene.camera.transform.translation.z += (camDirection.z * step) / S3L_F;
}

void Renderer::rotate_camera(int x, int y, int z)
{
    scene.camera.transform.rotation.x += x;
    scene.camera.transform.rotation.y += y;
    scene.camera.transform.rotation.z += z;
}

void Renderer::set_camera_pos(int x, int y, int z)
{
    scene.camera.transform.translation.x = x;
    scene.camera.transform.translation.y = y;
    scene.camera.transform.translation.z = z;
}

void Renderer::set_camera_rotation(int x, int y, int z)
{
    scene.camera.transform.rotation.x = x;
    scene.camera.transform.rotation.y = y;
    scene.camera.transform.rotation.z = z;
}

void Renderer::update()
{
    // eadk_display_push_rect_uniform(eadk_screen_rect, 0xFFFF);

    clear_z_buffer_bit_map();
    S3L_newFrame();
    S3L_drawScene(scene);

    fill_void_space(eadk_color_white);

    /* scene.camera.transform.rotation.y += 1;
    scene.models[0].transform.rotation.y += 10;
    scene.models[0].transform.rotation.x += 4;
    scene.models[0].transform.translation.x = S3L_sin(imageCount * 4);
    scene.models[0].transform.translation.y = S3L_sin(imageCount * 2) / 2;*/

    if (imageCount == 100)
    {
        eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_red);
        eadk_timing_msleep(2000);
        change_view_distance(1);
        eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_blue);
        eadk_timing_msleep(2000);
    }

    eadk_display_wait_for_vblank();

    imageCount++;
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

    // THERE IS AN ERROR IN THE FOLLOWING CODE THAT MAKE THE APP INSTANT CRASH AT LAUNCH
    /*
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
    */
    int triangle_count = triangles_list.size;

    S3L_Unit *verticies = (S3L_Unit *)malloc(triangle_count * 9); // 9 int per triangle
    S3L_Index *triangles = (S3L_Index *)malloc(triangle_count);

    for (int i = 0; i < triangle_count; i++)
    {
        triangle current_triangle = pop_front(&triangles_list);

        verticies[i * 9] = chunk->get_pos_x()*CHUNK_SIZE + current_triangle.x1;
        verticies[i * 9 + 1] = chunk->get_pos_y()*CHUNK_SIZE + current_triangle.y1;
        verticies[i * 9 + 2] = chunk->get_pos_z()*CHUNK_SIZE + current_triangle.z1;
        triangles[i * 3] = i * 3;

        verticies[i * 9 + 3] = chunk->get_pos_x()*CHUNK_SIZE + current_triangle.x2;
        verticies[i * 9 + 4] = chunk->get_pos_y()*CHUNK_SIZE + current_triangle.y2;
        verticies[i * 9 + 5] = chunk->get_pos_z()*CHUNK_SIZE + current_triangle.z2;
        triangles[i * 3 + 1] = i * 3 + 1;

        verticies[i * 9 + 6] = chunk->get_pos_x()*CHUNK_SIZE + current_triangle.x3;
        verticies[i * 9 + 7] = chunk->get_pos_y()*CHUNK_SIZE + current_triangle.y3;
        verticies[i * 9 + 8] = chunk->get_pos_z()*CHUNK_SIZE + current_triangle.z3;
        triangles[i * 3 + 2] = i * 3 + 2;
    }

    model->triangles = triangles;
    model->triangleCount = triangle_count;
    model->vertexCount = triangle_count * 9;
    model->vertices = verticies;

    clear(&triangles_list);
}