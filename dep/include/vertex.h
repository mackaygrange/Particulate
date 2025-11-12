#ifndef VERTEX_H
#define VERTEX_H

#include "config.h"

typedef struct vertex
{
	f32 x, y, z, w;

} vertex;


void vertex_set(vertex* v, f32 x, f32 y, f32 z, f32 w);

void vertex_add(vertex* v1, vertex* v2, vertex* v_o);
void vertex_subtract(vertex* v1, vertex* v2, vertex* v_o);
void vertex_multiply(vertex* v1, vertex* v2, vertex* v_o);
void vertex_divide(vertex* v1, vertex* v2, vertex* v_o);
void vertex_add_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z);
void vertex_subtract_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z);
void vertex_multiply_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z);
void vertex_divide_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z);

void vertex_normalize(vertex* v, vertex* v_o);
void vertex_cross_product(vertex* v1, vertex* v2, vertex* v_o);

f32 vertex_dot_product(vertex* v1, vertex* v2);
f32 vertex_length(vertex* v);

#endif