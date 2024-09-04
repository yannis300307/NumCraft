#include <stdint.h>

typedef
#if S3L_USE_WIDER_TYPES
  int64_t    
#else
  int32_t 
#endif
  S3L_Unit;

typedef 
#if S3L_USE_WIDER_TYPES
  int32_t 
#else
  int16_t
#endif
  S3L_ScreenCoord;

typedef 
#if S3L_USE_WIDER_TYPES
  uint32_t
#else
  uint16_t
#endif
  S3L_Index;

typedef struct
{
  S3L_Unit x;
  S3L_Unit y;
  S3L_Unit z;
  S3L_Unit w;
} S3L_Vec4;

typedef struct
{
  uint8_t backfaceCulling;    /**< What backface culling to use. Possible
                                   values:
                                   - 0 none
                                   - 1 clock-wise
                                   - 2 counter clock-wise */
  int8_t visible;             /**< Can be used to easily hide the model. */
} S3L_DrawConfig;

typedef S3L_Unit S3L_Mat4[4][4];

typedef struct
{
  S3L_Vec4 translation;
  S3L_Vec4 rotation; /**< Euler angles. Rortation is applied in this order:
                          1. z = by z (roll) CW looking along z+
                          2. x = by x (pitch) CW looking along x+
                          3. y = by y (yaw) CW looking along y+ */
  S3L_Vec4 scale;
} S3L_Transform3D;

typedef struct
{
  const S3L_Unit *vertices;
  S3L_Index vertexCount;
  const S3L_Index *triangles;
  S3L_Index triangleCount;
  S3L_Transform3D transform;
  S3L_Mat4 *customTransformMatrix; /**< This can be used to override the
                                     transform (if != 0) with a custom
                                     transform matrix, which is more
                                     general. */
  S3L_DrawConfig config;
} S3L_Model3D;                ///< Represents a 3D model.