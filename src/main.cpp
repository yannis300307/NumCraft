#include <stdio.h>
#include <malloc.h>

extern "C" {
  #include <eadk.h>
}

#define S3L_PIXEL_FUNCTION draw_pixel
#define S3L_RESOLUTION_X 340
#define S3L_RESOLUTION_Y 240

#include "small3dlib.h"

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "NumCraft";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


S3L_Unit cubeVertices[] = { S3L_CUBE_VERTICES(S3L_F) };
S3L_Index cubeTriangles[] = { S3L_CUBE_TRIANGLES };

S3L_Model3D cubeModel;
S3L_Scene scene;


void draw_pixel(S3L_PixelInfo *pixel) {
  eadk_color_t color = 0x0;
  eadk_display_push_rect({(uint16_t)(pixel->x), (uint16_t)(pixel->y), 1, 1}, &color);
}

int main(int argc, char * argv[]) {
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

  int i = 0;
  uint64_t last_timer = 1;
  uint64_t current_timer = 0;
  float delta = 0.;

  while (1) {
    eadk_keyboard_state_t keyboardState = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(keyboardState, eadk_key_home)) {
      return 0;
    }

    S3L_newFrame();
    S3L_drawScene(scene);

    char count_str[20];

    current_timer = eadk_timing_millis();

    delta = (float)(current_timer-last_timer)/1000.;

    if (delta == 0) {delta = 0.00001;} // yeah ...

    sprintf(count_str, "%d", (int)(1/delta));

    count_str[19] = 0;

    eadk_point_t pos;
    pos.x = 10;
    pos.y = 10;

    eadk_display_draw_string(count_str, pos, false, 0x0000, 0xFFFF);

    //scene.camera.transform.rotation.y += 1;
    scene.models[0].transform.rotation.y += 10;
    scene.models[0].transform.rotation.x += 4;
    scene.models[0].transform.translation.x = S3L_sin(i * 4);
    scene.models[0].transform.translation.y = S3L_sin(i * 2) / 2;

    eadk_display_wait_for_vblank();
    
    eadk_timing_msleep(100);
    i++;
    last_timer = current_timer;
  }

  return 0;
}
