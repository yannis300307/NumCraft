#include "display.h"
#include "eadkpp.h"
#include "palette.h"
#include <stdio.h>

#define S3L_PIXEL_FUNCTION draw_pixel
#define S3L_RESOLUTION_X 320
#define S3L_RESOLUTION_Y 240

#include "small3dlib.h"

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "NumCraft";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


S3L_Unit cubeVertices[] = { S3L_CUBE_VERTICES(S3L_F) };
S3L_Index cubeTriangles[] = { S3L_CUBE_TRIANGLES };

S3L_Model3D cubeModel;
S3L_Scene scene;


void draw_pixel(S3L_PixelInfo *pixel) {
  EADK::Display::pushRectUniform(EADK::Rect{pixel->x, pixel->y, 1, 1}, Black);
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
    EADK::Keyboard::State keyboardState = EADK::Keyboard::scan();
    if (keyboardState.keyDown(EADK::Keyboard::Key::Home)) {
      return 0;
    }

    EADK::Display::pushRectUniform(EADK::Screen::Rect, Yellow);

    S3L_newFrame();
    S3L_drawScene(scene);

    char count_str[20];

    current_timer = eadk_timing_millis();

    delta = (current_timer-last_timer);

    if (delta == 0) {delta = 0.00001;} // yeah ...

    sprintf(count_str, "%d", (int)(1/delta));

    count_str[19] = 0;

    eadk_point_t pos;
    pos.x = 10;
    pos.y = 10;

    eadk_display_draw_string(count_str, pos, false, Black, White);

    //scene.camera.transform.rotation.y += 1;
    scene.models[0].transform.rotation.y += 10;
    scene.models[0].transform.rotation.x += 4;
    scene.models[0].transform.translation.x = S3L_sin(i * 4);
    scene.models[0].transform.translation.y = S3L_sin(i * 2) / 2;

    eadk_display_wait_for_vblank();
    
    EADK::Timing::msleep(500);
    i++;
    last_timer = current_timer;
  }

  EADK::Display::pushRectUniform(EADK::Screen::Rect, LightBlue);
  return 0;
}
