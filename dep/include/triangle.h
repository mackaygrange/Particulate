#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "config.h"
#include "vertex.h"
#include "matrix.h"

typedef struct triangle
{
	vertex v[3];
} triangle;

void triangle_add_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z);
void triangle_subtract_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z);
void triangle_multiply_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z);
void triangle_divide_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z);

void triangle_normalize(triangle* t, triangle* t_o);
void triangle_cross_product(triangle* t, vertex* v_o);

void triangle_project(triangle* t, triangle* t_o, matrix* m_p);

void triangle_render(SDL_Renderer* renderer, triangle* t, SDL_Color c);
void triangle_render_fill(SDL_Renderer* renderer, triangle* t, SDL_Color c);

int compare_triangles(const void* t1, const void* t2);

#endif
