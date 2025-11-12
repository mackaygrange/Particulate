#include "matrix.h"

void matrix_set_identity(matrix* m)
{
	m->m[0][0] = 1.0f; 	m->m[0][1] = 0.0f; 	m->m[0][2] = 0.0f; 	m->m[0][3] = 0.0f;
	m->m[1][0] = 0.0f; 	m->m[1][1] = 1.0f; 	m->m[1][2] = 0.0f; 	m->m[1][3] = 0.0f;
	m->m[2][0] = 0.0f; 	m->m[2][1] = 0.0f; 	m->m[2][2] = 1.0f; 	m->m[2][3] = 0.0f;
	m->m[3][0] = 0.0f; 	m->m[3][1] = 0.0f; 	m->m[3][2] = 0.0f; 	m->m[3][3] = 1.0f;
}

void matrix_set_projection(matrix* m)
{
	matrix_set_identity(m);

	m->m[0][0] = 1.0f / (F_ASPECT_RATIO * tanf(F_FOV * 0.5f * DEG_RAD_RATIO));
	m->m[1][1] = 1.0f / tanf(F_FOV * 0.5f * DEG_RAD_RATIO);
	m->m[2][2] = F_FAR / (F_FAR - F_NEAR);
	m->m[3][2] = (-F_FAR * F_NEAR) / (F_FAR - F_NEAR);
	m->m[2][3] = 1.0f;
	m->m[3][3] = 0.0f;
}

void matrix_multiplication(matrix* m1, matrix* m2, matrix* m_o)
{
	matrix o = { 0 };

	for (int c = 0; c < 4; c++)
	{
		for (int r = 0; r < 4; r++)
		{
			o.m[r][c] = (m1->m[r][0] * m2->m[0][c]) + (m1->m[r][1] * m2->m[1][c]) + (m1->m[r][2] * m2->m[2][c]) + (m1->m[r][3] * m2->m[3][c]);
		}
	}
	*m_o = o;
}

void triangle_matrix_multiplication(triangle* t, triangle* t_o, matrix* m)
{
	for (int i = 0; i < 3; i++)
	{
		vertex_matrix_multiplication(&t->v[i], &t_o->v[i], m);
	}
}

void vertex_matrix_multiplication(vertex* v, vertex* v_o, matrix* m)
{
	vertex o = { 0 };

	o.x = v->x * m->m[0][0] + v->y * m->m[1][0] + v->z * m->m[2][0] + v->w * m->m[3][0];
	o.y = v->x * m->m[0][1] + v->y * m->m[1][1] + v->z * m->m[2][1] + v->w * m->m[3][1];
	o.z = v->x * m->m[0][2] + v->y * m->m[1][2] + v->z * m->m[2][2] + v->w * m->m[3][2];
	o.w = v->x * m->m[0][3] + v->y * m->m[1][3] + v->z * m->m[2][3] + v->w * m->m[3][3];

	*v_o = o;
}


void matrix_set_camera(matrix* m, vertex* pos, vertex* tar, vertex* up)
{
	matrix_set_point_at(m, pos, tar, up);
	matrix_q_invert(m);
}


void matrix_set_point_at(matrix* m, vertex* pos, vertex* tar, vertex* up)
{
	matrix_set_identity(m);

	vertex new_forward, new_up, new_right, a;

	vertex_subtract(tar, pos, &new_forward);
	vertex_normalize(&new_forward, &new_forward);

	f32 dot = vertex_dot_product(up, &new_forward);

	vertex_multiply_scalar(&new_forward, &a, dot, dot, dot);
	vertex_subtract(up, &a, &new_up);
	vertex_normalize(&new_up, &new_up);

	vertex_cross_product(&new_up, &new_forward, &new_right);

	m->m[0][0] = new_right.x;	m->m[0][1] = new_right.y;	m->m[0][2] = new_right.z;	m->m[0][3] = 0.0f;
	m->m[1][0] = new_up.x;		m->m[1][1] = new_up.y;		m->m[1][2] = new_up.z;		m->m[1][3] = 0.0f;
	m->m[2][0] = new_forward.x;	m->m[2][1] = new_forward.y;	m->m[2][2] = new_forward.z;	m->m[2][3] = 0.0f;
	m->m[3][0] = pos->x;		m->m[3][1] = pos->y;		m->m[3][2] = pos->z;		m->m[3][3] = 1.1f;
}

void matrix_q_invert(matrix* m)
{
	matrix m_t = { 0 };

	m_t.m[0][0] = m->m[0][0]; m_t.m[0][1] = m->m[1][0]; m_t.m[0][2] = m->m[2][0]; m_t.m[0][3] = 0.0f;
	m_t.m[1][0] = m->m[0][1]; m_t.m[1][1] = m->m[1][1]; m_t.m[1][2] = m->m[2][1]; m_t.m[1][3] = 0.0f;
	m_t.m[2][0] = m->m[0][2]; m_t.m[2][1] = m->m[1][2]; m_t.m[2][2] = m->m[2][2]; m_t.m[2][3] = 0.0f;
	m_t.m[3][0] = -(m->m[3][0] * m_t.m[0][0] + m->m[3][1] * m_t.m[1][0] + m->m[3][2] * m_t.m[2][0]);
	m_t.m[3][1] = -(m->m[3][0] * m_t.m[0][1] + m->m[3][1] * m_t.m[1][1] + m->m[3][2] * m_t.m[2][1]);
	m_t.m[3][2] = -(m->m[3][0] * m_t.m[0][2] + m->m[3][1] * m_t.m[1][2] + m->m[3][2] * m_t.m[2][2]);
	m_t.m[3][3] = 1.0f;

	*m = m_t;
}