#include "vertex.h"

void vertex_set(vertex* v, f32 x, f32 y, f32 z, f32 w)
{ 
	ASSERT(v != NULL, "Output cannot be NULL.");

	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void vertex_add(vertex* v1, vertex* v2, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v1->x + v2->x;
	v_o->y = v1->y + v2->y;
	v_o->z = v1->z + v2->z;
}

void vertex_subtract(vertex* v1, vertex* v2, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v1->x - v2->x;
	v_o->y = v1->y - v2->y;
	v_o->z = v1->z - v2->z;
}

void vertex_multiply(vertex* v1, vertex* v2, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v1->x * v2->x;
	v_o->y = v1->y * v2->y;
	v_o->z = v1->z * v2->z;
}

void vertex_divide(vertex* v1, vertex* v2, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v1->x / v2->x;
	v_o->y = v1->y / v2->y;
	v_o->z = v1->z / v2->z;
}

void vertex_add_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v->x + x;
	v_o->y = v->y + y;
	v_o->z = v->z + z;
}

void vertex_subtract_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v->x - x;
	v_o->y = v->y - y;
	v_o->z = v->z - z;
}

void vertex_multiply_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v->x * x;
	v_o->y = v->y * y;
	v_o->z = v->z * z;
}

void vertex_divide_scalar(vertex* v, vertex* v_o, f32 x, f32 y, f32 z)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = v->x / x;
	v_o->y = v->y / y;
	v_o->z = v->z / z;
}

void vertex_normalize(vertex* v, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	f32 len = vertex_length(v);
	vertex_divide_scalar(v, v_o, len, len, len);
}

void vertex_cross_product(vertex* v1, vertex* v2, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	v_o->x = (v1->y * v2->z) - (v1->z * v2->y);
	v_o->y = (v1->z * v2->x) - (v1->x * v2->z);
	v_o->z = (v1->x * v2->y) - (v1->y * v2->x);
}

f32 vertex_dot_product(vertex* v1, vertex* v2)
{
	return (f32) { (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z) };
}

f32 vertex_length(vertex* v)
{
	return (f32) { sqrtf(vertex_dot_product(v, v)) };
}