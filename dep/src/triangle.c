#include "triangle.h"

void triangle_add_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z)
{
	ASSERT(t_o != NULL, "Output cannot be NULL.");

	for (int v = 0; v < 3; v++)
	{
		vertex_add_scalar(&t->v[v], &t_o->v[v], x, y, z);
	}
}

void triangle_subtract_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z)
{
	ASSERT(t_o != NULL, "Output cannot be NULL.");

	for (int v = 0; v < 3; v++)
	{
		vertex_subtract_scalar(&t->v[v], &t_o->v[v], x, y, z);
	}
}

void triangle_multiply_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z)
{
	ASSERT(t_o != NULL, "Output cannot be NULL.");

	for (int v = 0; v < 3; v++)
	{
		vertex_multiply_scalar(&t->v[v], &t_o->v[v], x, y, z);
	}
}

void triangle_divide_scalar(triangle* t, triangle* t_o, f32 x, f32 y, f32 z)
{
	ASSERT(t_o != NULL, "Output cannot be NULL.");

	for (int v = 0; v < 3; v++)
	{
		vertex_divide_scalar(&t->v[v], &t_o->v[v], x, y, z);
	}
}

void triangle_project(triangle* t, triangle* t_o, matrix* m_p)
{
	ASSERT(t_o != NULL, "Output cannot be NULL.");

	for(int v = 0; v < 3; v++)
	{
		vertex_set(&t->v[v], t->v[v].x, t->v[v].y, t->v[v].z, 1.0f);
	}

	triangle_matrix_multiplication(t, t_o, m_p);
	
	for (int v = 0; v < 3; v++)
	{
		if (t_o->v[v].w != 0.0f)
		{
			vertex_divide_scalar(&t_o->v[v], &t_o->v[v], t_o->v[v].w, t_o->v[v].w, t_o->v[v].w);
		}
	}
}

void triangle_normalize(triangle* t, triangle* t_o)
{
	ASSERT(t_o != NULL, "Output cannot be NULL.");

	for (int v = 0; v < 3; v++)
	{
		vertex_normalize(&t->v[v], &t_o->v[v]);
	}
}

void triangle_cross_product(triangle* t, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	vertex line_1, line_2, normal;

	vertex_subtract(&t->v[1], &t->v[0], &line_1);
	vertex_subtract(&t->v[2], &t->v[0], &line_2);

	vertex_cross_product(&line_1, &line_2, &normal);
	vertex_normalize(&normal, &normal);

	*v_o = normal;
}

void triangle_render(SDL_Renderer* renderer, triangle* t, SDL_Color c)
{
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	SDL_RenderDrawLine(renderer,
		((int)(t->v[0].x)),
		((int)(t->v[0].y)),
		((int)(t->v[1].x)),
		((int)(t->v[1].y))
	);

	SDL_RenderDrawLine(renderer,
		((int)(t->v[1].x)),
		((int)(t->v[1].y)),
		((int)(t->v[2].x)),
		((int)(t->v[2].y))
	);

	SDL_RenderDrawLine(renderer,
		((int)(t->v[2].x)),
		((int)(t->v[2].y)),
		((int)(t->v[0].x)),
		((int)(t->v[0].y))
	);
}

void triangle_render_fill(SDL_Renderer* renderer, triangle* t, SDL_Color c)
{
	SDL_FPoint point_1 = { t->v[0].x, t->v[0].y };
	SDL_FPoint point_2 = { t->v[1].x, t->v[1].y };
	SDL_FPoint point_3 = { t->v[2].x, t->v[2].y };
	SDL_FPoint null = { 0 };

	SDL_Vertex vertices[3] =
	{
		{point_1, c, null},
		{point_2, c, null},
		{point_3, c, null}
	};

	SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
}

int compare_triangles(const void* t1, const void* t2)
{
	triangle t_1 = *((triangle*)t1);
	triangle t_2 = *((triangle*)t2);

	f32 z1 = (t_1.v[0].z + t_1.v[1].z + t_1.v[2].z) / 3.0f;
	f32 z2 = (t_2.v[0].z + t_2.v[1].z + t_2.v[2].z) / 3.0f;

	if (z1 < z2) return -1;
	if (z1 > z2) return 1;
	return 0;
}
