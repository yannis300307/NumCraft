#include "renderer.hpp"


void draw_pixel(S3L_PixelInfo *pixel) {
  eadk_color_t color = 0x0;
  eadk_display_push_rect({(uint16_t)(pixel->x), (uint16_t)(pixel->y), 1, 1}, &color);
}

Renderer::Renderer(Core* core)
{
    this->core = core;
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

void Renderer::update() {
    eadk_display_push_rect_uniform(eadk_screen_rect, 0xFFFF);

    S3L_newFrame();
    S3L_drawScene(scene);

    //scene.camera.transform.rotation.y += 1;
    scene.models[0].transform.rotation.y += 10;
    scene.models[0].transform.rotation.x += 4;
    scene.models[0].transform.translation.x = S3L_sin(imageCount * 4);
    scene.models[0].transform.translation.y = S3L_sin(imageCount * 2) / 2;

    eadk_display_wait_for_vblank();

    imageCount++;
}
