#ifndef MATRIX_H
#define MATRIX_H

#include "config.h"
#include "triangle.h"
#include "vertex.h"

typedef struct matrix
{ 
	f32 m[4][4]; 
} matrix;

void matrix_set_identity(matrix* m);
void matrix_set_projection(matrix* m);

void matrix_multiplication(matrix* m1, matrix* m2, matrix* m_o);
void triangle_matrix_multiplication(triangle* t, triangle* t_o, matrix* m);
void vertex_matrix_multiplication(vertex* v, vertex* v_o, matrix* m);

void matrix_set_camera(matrix* m, vertex* pos, vertex* tar, vertex* up);
void matrix_set_point_at(matrix* m, vertex* pos, vertex* tar, vertex* up);
void matrix_q_invert(matrix* m);

#endif