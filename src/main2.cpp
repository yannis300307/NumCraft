/*
  Example for small3dlib: model viewer. See the program's help for more info.

  author: Miloslav Ciz
  license: CC0
*/

#include <math.h>

#include "display.h"
#include "eadk.h"
#include "palette.h"

#define S3L_RESOLUTION_X 320
#define S3L_RESOLUTION_Y 240

#define S3L_FLAT 0
#define S3L_NEAR_CROSS_STRATEGY 0
#define S3L_PERSPECTIVE_CORRECTION 2
#define S3L_SORT 0
#define S3L_STENCIL_BUFFER 0
#define S3L_Z_BUFFER 1

#define S3L_PIXEL_FUNCTION drawPixel


#include "small3dlib.h"

#define TEXTURE_W 128
#define TEXTURE_H 128

#include "smal3dlib/programs/houseTexture.h"
#include "smal3dlib/programs/houseModel.h"

#include "smal3dlib/programs/chestTexture.h"
#include "smal3dlib/programs/chestModel.h"

#include "smal3dlib/programs/plantTexture.h"
#include "smal3dlib/programs/plantModel.h"

#include "smal3dlib/programs/cat1Model.h"
#include "smal3dlib/programs/cat2Model.h"
#include "smal3dlib/programs/catTexture.h"

#define MODE_TEXTUERED 0
#define MODE_SINGLE_COLOR 1
#define MODE_NORMAL_SMOOTH 2
#define MODE_NORMAL_SHARP 3
#define MODE_BARYCENTRIC 4
#define MODE_TRIANGLE_INDEX 5

void setPixel(int x, int y, int r, int g, int b) {
  EADK::Display::pushRectUniform(EADK::Rect{x, y, 1, 1}, Black);
}


S3L_Unit houseNormals[HOUSE_VERTEX_COUNT * 3];
S3L_Unit chestNormals[CHEST_VERTEX_COUNT * 3];
S3L_Unit catNormals[CAT1_VERTEX_COUNT * 3];
S3L_Unit plantNormals[PLANT_VERTEX_COUNT * 3];

S3L_Unit catVertices[CAT1_VERTEX_COUNT * 3];
const S3L_Index *catTriangleIndices = cat1TriangleIndices;
const S3L_Unit *catUVs = cat1UVs;
const S3L_Index *catUVIndices = cat1UVIndices;

S3L_Model3D catModel; 

S3L_Model3D model;
const uint8_t *texture;
const S3L_Unit *uvs;
const S3L_Unit *normals;
const S3L_Index *uvIndices;

S3L_Scene scene;

uint32_t frame = 0;

void animate(double time)
{
  time = (1.0 + sin(time * 8)) / 2; 

  S3L_Unit t = time * S3L_F;

  for (S3L_Index i = 0; i < CAT1_VERTEX_COUNT * 3; i += 3)
  {
    S3L_Unit v0[3], v1[3];

    v0[0] = cat1Vertices[i];
    v0[1] = cat1Vertices[i + 1];
    v0[2] = cat1Vertices[i + 2];

    v1[0] = cat2Vertices[i];
    v1[1] = cat2Vertices[i + 1];
    v1[2] = cat2Vertices[i + 2];

    catVertices[i] =  S3L_interpolateByUnit(v0[0],v1[0],t);
    catVertices[i + 1] = S3L_interpolateByUnit(v0[1],v1[1],t);
    catVertices[i + 2] = S3L_interpolateByUnit(v0[2],v1[2],t);
  }
}

uint32_t previousTriangle = -1;
S3L_Vec4 uv0, uv1, uv2;
uint16_t l0, l1, l2;
S3L_Vec4 toLight;
int8_t light = 1;
int8_t fog = 0;
int8_t noise = 0;
int8_t wire = 0;
int8_t transparency = 0;
int8_t mode = 0;
S3L_Vec4 n0, n1, n2, nt;

void drawPixel(S3L_PixelInfo *p)
{
  if (p->triangleID != previousTriangle)
  {
    if (mode == MODE_TEXTUERED)
    {
      S3L_getIndexedTriangleValues(p->triangleIndex,uvIndices,uvs,2,&uv0,&uv1,&uv2);
    }
    else if (mode == MODE_NORMAL_SHARP)
    {
      S3L_Vec4 v0, v1, v2;
      S3L_getIndexedTriangleValues(p->triangleIndex,model.triangles,model.vertices,3,&v0,&v1,&v2);

      S3L_triangleNormal(v0,v1,v2,&nt);

      nt.x = S3L_clamp(128 + nt.x / 4,0,255);
      nt.y = S3L_clamp(128 + nt.y / 4,0,255);
      nt.z = S3L_clamp(128 + nt.z / 4,0,255);
    }

    if (light || mode == MODE_NORMAL_SMOOTH)
    {
      S3L_getIndexedTriangleValues(p->triangleIndex,model.triangles,normals,3,&n0,&n1,&n2);

      l0 = 256 + S3L_clamp(S3L_vec3Dot(n0,toLight),-511,511) / 2;
      l1 = 256 + S3L_clamp(S3L_vec3Dot(n1,toLight),-511,511) / 2;
      l2 = 256 + S3L_clamp(S3L_vec3Dot(n2,toLight),-511,511) / 2;
    }

    previousTriangle = p->triangleID;
  }

  if (wire)
    if (p->barycentric[0] != 0 &&
        p->barycentric[1] != 0 &&
        p->barycentric[2] != 0)
      return;

  uint8_t
    r = 0,
    g = 0,
    b = 0;

  int8_t transparent = 0;

  switch (mode)
  {
    case MODE_TEXTUERED:
    {
      
      break;
    }

    case MODE_SINGLE_COLOR:
    {
      r = 128;
      g = 128;
      b = 128;

      break;
    }

    case MODE_NORMAL_SMOOTH:
    {
      S3L_Vec4 n;

      n.x = S3L_interpolateBarycentric(n0.x,n1.x,n2.x,p->barycentric);
      n.y = S3L_interpolateBarycentric(n0.y,n1.y,n2.y,p->barycentric);
      n.z = S3L_interpolateBarycentric(n0.z,n1.z,n2.z,p->barycentric);

      S3L_vec3Normalize(&n);

      r = S3L_clamp(128 + n.x / 4,0,255);
      g = S3L_clamp(128 + n.y / 4,0,255);
      b = S3L_clamp(128 + n.z / 4,0,255);
 
      break;
    }

    case MODE_NORMAL_SHARP:
    {
      r = nt.x;
      g = nt.y;
      b = nt.z;
      break;
    }

    case MODE_BARYCENTRIC:
    {
      r = p->barycentric[0] >> 1;
      g = p->barycentric[1] >> 1;
      b = p->barycentric[2] >> 1;
      break;
    }

    case MODE_TRIANGLE_INDEX:
    {
      r = S3L_min(p->triangleIndex,255);
      g = r;
      b = r;
    }
  
    default:
      break;
  }

  if (light)
  {
    int16_t l = S3L_interpolateBarycentric(l0,l1,l2,p->barycentric);

    r = S3L_clamp((((int16_t) r) * l) / S3L_F,0,255);
    g = S3L_clamp((((int16_t) g) * l) / S3L_F,0,255);
    b = S3L_clamp((((int16_t) b) * l) / S3L_F,0,255);
  }

  if (fog)
  {
    int16_t f = ((p->depth - S3L_NEAR) * 255) / (S3L_F * 64);

    f *= 2;

    r = S3L_clamp(((int16_t) r) + f,0,255);
    g = S3L_clamp(((int16_t) g) + f,0,255);
    b = S3L_clamp(((int16_t) b) + f,0,255);
  }

  if (transparency && transparent)
  {
    S3L_zBufferWrite(p->x,p->y,p->previousZ);
    return;
  }

  if (noise)
    setPixel(p->x + rand() % 8,p->y + rand() % 8,r,g,b); 
  else
    setPixel(p->x,p->y,r,g,b); 
}

void draw(void)
{
  S3L_newFrame();

  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);
  S3L_drawScene(scene);
}

void setModel(uint8_t index)
{
  #define modelCase(n,m)\
    case n:\
    {\
      texture = m##Texture;\
      uvs = m##UVs;\
      uvIndices = m##UVIndices;\
      normals = m##Normals;\
      scene.models[0] = m##Model;\
      S3L_computeModelNormals(scene.models[0],m##Normals,0);\
      break;\
    }

  switch (index)
  {
    modelCase(0,house)
    modelCase(1,chest)
    modelCase(2,cat)
    modelCase(3,plant)

    default:
      break;
  }

  #undef modelCase

  S3L_transform3DInit(&(scene.models[0].transform));
  S3L_drawConfigInit(&(scene.models[0].config));

  if (index == 3)
  {
    scene.models[0].config.backfaceCulling = 0;
    transparency = 1;
  }
  else
  {
    scene.models[0].config.backfaceCulling = 2;
    transparency = 0;
  }
}

int16_t fps = 0;

int main(void)
{

  toLight.x = 10;
  toLight.y = 10;
  toLight.z = 10;

  S3L_vec3Normalize(&toLight);

  S3L_sceneInit(&model,1,&scene);

  houseModelInit();
  chestModelInit();
  plantModelInit();
  cat1ModelInit();
  cat2ModelInit();

  scene.camera.transform.translation.z = -S3L_F * 8;

  catModel = cat1Model;
  catModel.vertices = catVertices;
  animate(0);

  int8_t modelIndex = 0;
  int8_t modelsTotal = 4;
  setModel(0);

  int running = 1;


  while (running)
  {

    draw();

    fps++;


    double timeDiff = 0.1;
    double frameDiff = 0.1;

    if (timeDiff >= 1.0)
    {
      fps = 0;
    }

    int keyboardState = eadk_keyboard_scan();
    
    if (eadk_keyboard_key_down(keyboardState, eadk_key_home)) {
      running = 0;
    }
    if (eadk_keyboard_key_down(keyboardState, eadk_key_power)) {
      fog = !fog;
    }
    if (eadk_keyboard_key_down(keyboardState, eadk_key_square)) {
      light = !light;
    }
    if (eadk_keyboard_key_down(keyboardState, eadk_key_square)) {
      noise = !noise;
    }
    if (eadk_keyboard_key_down(keyboardState, eadk_key_ok)) {
      modelIndex = (modelIndex + 1) % modelsTotal;
      setModel(modelIndex);
    }

    int16_t rotationStep = S3L_max(1,300 * frameDiff);
    int16_t moveStep = S3L_max(1,3000 * frameDiff);
    int16_t fovStep = S3L_max(1,1000 * frameDiff);

    if (!eadk_keyboard_key_down(keyboardState, eadk_key_shift))
    {
      if (eadk_keyboard_key_down(keyboardState, eadk_key_left))
        model.transform.rotation.y += rotationStep;
      else if (eadk_keyboard_key_down(keyboardState, eadk_key_right))
        model.transform.rotation.y -= rotationStep;
      
      if (eadk_keyboard_key_down(keyboardState, eadk_key_down))
        model.transform.rotation.x += rotationStep;
      else if (eadk_keyboard_key_down(keyboardState, eadk_key_up))
        model.transform.rotation.x -= rotationStep;
    }
    else
    {
      if (eadk_keyboard_key_down(keyboardState, eadk_key_left))
        scene.camera.focalLength =
          S3L_min(S3L_F * 5,scene.camera.focalLength + fovStep);
      else if (eadk_keyboard_key_down(keyboardState, eadk_key_right))
        scene.camera.focalLength =
          S3L_max(S3L_F / 2,scene.camera.focalLength - fovStep);

      if (eadk_keyboard_key_down(keyboardState, eadk_key_up))
        scene.camera.transform.translation.z =
          S3L_min(S3L_F, scene.camera.transform.translation.z + moveStep);
      else if (eadk_keyboard_key_down(keyboardState, eadk_key_down))
        scene.camera.transform.translation.z =
          S3L_max(-S3L_F * 16, scene.camera.transform.translation.z - moveStep);
    }
    
    if (eadk_keyboard_key_down(keyboardState, eadk_key_one))
      mode = MODE_TEXTUERED;
    else if (eadk_keyboard_key_down(keyboardState, eadk_key_two))
      mode = MODE_SINGLE_COLOR;
    else if (eadk_keyboard_key_down(keyboardState, eadk_key_three))
      mode = MODE_NORMAL_SMOOTH;
    else if (eadk_keyboard_key_down(keyboardState, eadk_key_four))
      mode = MODE_NORMAL_SHARP;
    else if (eadk_keyboard_key_down(keyboardState, eadk_key_five))
      mode = MODE_BARYCENTRIC;
    else if (eadk_keyboard_key_down(keyboardState, eadk_key_six))
      mode = MODE_TRIANGLE_INDEX;

    if (modelIndex == 2)
      animate(0.1); 

    frame++;
  }

  return 0;
}
