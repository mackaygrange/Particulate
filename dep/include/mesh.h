#ifndef MESH_H
#define MESH_H

#include "config.h"
#include "matrix.h"
#include "triangle.h"
#include "vertex.h"

typedef struct mesh
{
	triangle t[1000];
	int c;
    char flag;
} mesh;

void mesh_render(SDL_Renderer* renderer, mesh* m, matrix* m_p, matrix* m_c, vertex* light_dir);

void mesh_add_tile(mesh* m, f32 x, f32 y, f32 z);
void mesh_init_cube(mesh* m);
int load_from_txt_file(char* file_name, mesh* empty_mesh);

#endif
