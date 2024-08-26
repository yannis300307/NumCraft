#include "renderer.hpp"
#include "texture_reader.hpp"
#include "textures/tileset.h"

#define S3L_PIXEL_FUNCTION draw_pixel
#define S3L_RESOLUTION_X 320
#define S3L_RESOLUTION_Y 240
#define PIXEL_COUNT (S3L_RESOLUTION_X*S3L_RESOLUTION_Y)
#define Z_BUFFER_BIT_MAP (S3L_RESOLUTION_X * S3L_RESOLUTION_Y / 8)

#include "libs/small3dlib.h"
#include <bitset>

using namespace std;

bitset<PIXEL_COUNT> z_buffer_bit_map;

S3L_Unit cubeVertices[] = {S3L_CUBE_VERTICES(S3L_F)};
S3L_Index cubeTriangles[] = {S3L_CUBE_TRIANGLES};
S3L_Unit cubeUvs[] = {S3L_CUBE_TEXCOORDS(16)};
S3L_Index cubeuvIndices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

S3L_Model3D cubeModel;
S3L_Scene scene;

uint32_t previousTriangle = -1;
S3L_Vec4 uv0, uv1, uv2;
const uint8_t *texture = debug_texture;
const S3L_Index *uvIndices = cubeuvIndices;
const S3L_Unit *uvs = cubeUvs;

void clear_z_buffer_bit_map()
{
    /*Clear the the bit map z buffer.*/
    z_buffer_bit_map.reset();
}

void draw_pixel(S3L_PixelInfo *pixel)
{
    /*Draw the pixel gived by the rasterizer.*/
    if (pixel->triangleID != previousTriangle)
    {
        S3L_getIndexedTriangleValues(pixel->triangleIndex, uvIndices, uvs, 2, &uv0, &uv1, &uv2);
        previousTriangle = pixel->triangleID;
    }

    S3L_Unit uv[2];
    uint8_t
    r = 0,
    g = 0,
    b = 0;

    uv[0] = S3L_interpolateBarycentric(uv0.x,uv1.x,uv2.x,pixel->barycentric);
    uv[1] = S3L_interpolateBarycentric(uv0.y,uv1.y,uv2.y,pixel->barycentric);

    sampleTexture(texture,uv[0],uv[1],&r,&g,&b);

    eadk_color_t color = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
    eadk_display_push_rect({(uint16_t)(pixel->x), (uint16_t)(pixel->y), 1, 1}, &color);

    // Set the z bit map buffer.
    z_buffer_bit_map.set(pixel->x + pixel->y * S3L_RESOLUTION_X);
}


Renderer::Renderer()
{
    scene = S3L_Scene();
    S3L_model3DInit(
        cubeVertices,
        S3L_CUBE_VERTEX_COUNT,
        cubeTriangles,
        S3L_CUBE_TRIANGLE_COUNT,
        &cubeModel);

    S3L_sceneInit( // Initialize the scene we'll be rendering.
    &cubeModel,  // This is like an array with only one model in it.
    1,
    &scene);

    scene.camera.transform.translation.z = -2 * S3L_F;
}

void fill_void_space(eadk_color_t color)
{
    /*Fill the screen where triangles are not rendered on the last frame depending of the bit map z buffer.*/

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

void Renderer::update()
{
    // eadk_display_push_rect_uniform(eadk_screen_rect, 0xFFFF);

    clear_z_buffer_bit_map();
    S3L_newFrame();
    S3L_drawScene(scene);
    
    fill_void_space(eadk_color_white);

    //scene.camera.transform.rotation.y += 1;
    scene.models[0].transform.rotation.y += 10;
    scene.models[0].transform.rotation.x += 4;
    scene.models[0].transform.translation.x = S3L_sin(imageCount * 4);
    scene.models[0].transform.translation.y = S3L_sin(imageCount * 2) / 2;
    
    eadk_display_wait_for_vblank();

    imageCount++;
}
