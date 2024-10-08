#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "chunk.hpp"

typedef struct S3L_Model3D S3L_Model3D;

void build_chunk_mesh(Chunk *chunk, S3L_Model3D *models);

#endif
