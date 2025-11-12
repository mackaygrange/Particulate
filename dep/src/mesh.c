#include "mesh.h"

static void set_grey_scale(SDL_Color* color, f32 dot)
{
	color->r = (int)(127.5 * dot);
	color->g = (int)(127.5 * dot);
	color->b = (int)(127.5 * dot);
}

void mesh_render(SDL_Renderer* renderer, mesh* m, matrix* m_p, matrix* m_c, vertex* light_dir)
{
    //SDL_Color white = { .r = 255, .g = 255, .b = 255, .a = 255 };
	SDL_Color shade;
    triangle t_t;
    vertex normal;
    f32 shader_f;

	for (int t = 0; t < m->c; t++)
	{
		t_t = m->t[t];

		//triangle_add_scalar(&t_t, &t_t, 0.0f, 0.0f, 25.0f);

        triangle_cross_product(&t_t, &normal);
        shader_f = vertex_dot_product(&normal, light_dir);
        set_grey_scale(&shade, shader_f);

        if (m->flag == 'p') 
        {
            shade.r = 255;
            shade.g = 100;
            shade.b = 50;
        }

		triangle_matrix_multiplication(&t_t, &t_t, m_c);
		triangle_project(&t_t, &t_t, m_p);

		// This line of code creates a fish eye effect, where as objects disappear out of sight they instead seemingly rotate around a sphere.
		//triangle_normalize(&t_t, &t_t);
		
		triangle_add_scalar(&t_t, &t_t, 1.0f, 1.0f, 0.0f);
		triangle_multiply_scalar(&t_t, &t_t, (f32)SCREEN_WIDTH * 0.5f, (f32)SCREEN_HEIGHT * 0.5f, 0.0f);

		triangle_render_fill(renderer, &t_t, shade);
	}
}


//
//static void set_purple_scale(SDL_Color* color, f32 dot)
//{
//	color->r = (int)(127.5 * dot);
//	color->g = (int)(25 * dot);
//	color->b = (int)(200 * dot);
//}

//void render_mesh(SDL_Renderer* renderer, mesh* m, v3* camera, v3* light_direction)
//{
//	// Initialize method variables:
//	f32 dot;
//	v3 normal, camera_ray = { 0 };
//	triangle t_p = { 0 };
//	matrix m_p = { 0 };
//	SDL_Color c = { 0, 0, 0, 255 };
//
//	// Init up to date projection matrix:
//	matrix_init_projection(&m_p, F_FOV, F_ASPECT_RATIO, F_NEAR, F_FAR);
//
//	// Sort mesh to ensure organized state in regards to average z depth:
//	qsort(m->triangles, m->triangle_c, sizeof(triangle), compare_triangles);
//
//	for (int t = 0; t < m->triangle_c; t++)
//	{
//		// Copy triangle info into a temporary triangle:
//		t_p = m->triangles[t];
//
//		// Displace the temporary triangle along the z axis for viewing:
//		triangle_add_f_in(&t_p, 0.0f, 0.0f, -10.0);
//
//		// Calculate and normalize the normal and camera ray for comparison: 
//		normal = triangle_cross_product(&t_p);
//		camera_ray = v3_subtract(&t_p.points[0], camera);
//		camera_ray = v3_normalize(&camera_ray);
//
//		// Cull triangles (set to TRUE for no culling, poor handling of rending the inside of shapes currently.): 
//		if (v3_dot_product(&normal, &camera_ray) < 0.0f || TRUE)
//		{
//			// Project triangle:
//			triangle_project_in(&t_p, &m_p);
//
//			// Normalize projected triangle:
//			triangle_normalize_in(&t_p);
//
//			// Adjust the x and y values by adding 1 to the normalized values within the range of [-1, 1], ensuring numbers within the range of [0, 2]. We can then scale this value according to the center point of the screen.
//			triangle_add_f_in(&t_p, 1.0f, 1.0f, 0.0f);
//			triangle_multiply_f_in(&t_p, (f32)CENTER_SCREEN_WIDTH, (f32)CENTER_SCREEN_HEIGHT, 1.0f);
//
//			// Calculate color/lighting information:
//			dot = v3_dot_product(&normal, light_direction);
//			set_purple_scale(&c, (f32)fabs(dot));
//
//			// Draw the projected triangle with the calculated color:
//			render_triangle_fill(renderer, &t_p, c);
//		}
//	}
//}

void mesh_add_tile(mesh* m, f32 x, f32 y, f32 z)
{
	m->t[m->c].v[0].x = x;
	m->t[m->c].v[0].y = y;
	m->t[m->c].v[0].z = z;
	m->t[m->c].v[0].w = 1.0f;

	m->t[m->c].v[1].x = x + 1.0f;
	m->t[m->c].v[1].y = y;
	m->t[m->c].v[1].z = z;
	m->t[m->c].v[1].w = 1.0f;

	m->t[m->c].v[2].x = x;
	m->t[m->c].v[2].y = y + 1.0f;
	m->t[m->c].v[2].z = z;
	m->t[m->c].v[2].w = 1.0f;

	m->c++;

	m->t[m->c].v[0].x = x + 1.0f;
	m->t[m->c].v[0].y = y + 1.0f;
	m->t[m->c].v[0].z = z;
	m->t[m->c].v[0].w = 1.0f;

	m->t[m->c].v[1].x = x;
	m->t[m->c].v[1].y = y + 1.0f;
	m->t[m->c].v[1].z = z;
	m->t[m->c].v[1].w = 1.0f;

	m->t[m->c].v[2].x = x + 1.0f;
	m->t[m->c].v[2].y = y;
	m->t[m->c].v[2].z = z;
	m->t[m->c].v[2].w = 1.0f;

	m->c++;
}

//void mesh_init_cube(mesh* m)
//{
//	m->t[0] = (triangle){ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f };
//	m->t[1] = (triangle){ 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f };
//	m->t[2] = (triangle){ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f };
//	m->t[3] = (triangle){ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f };
//	m->t[4] = (triangle){ 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f };
//	m->t[5] = (triangle){ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f };
//	m->t[6] = (triangle){ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f };
//	m->t[7] = (triangle){ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f };
//	m->t[8] = (triangle){ 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f };
//	m->t[9] = (triangle){ 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f };
//	m->t[10] = (triangle){ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f };
//	m->t[11] = (triangle){ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f };
//
//	m->c = 12;
//}

//int load_from_txt_file(char* file_name, mesh* empty_mesh)
//{
//	FILE* input_file;
//	vertex* vertices = (vertex*)malloc(MAX_VERTICES * sizeof(vertex));
//	triangle* triangles = (triangle*)malloc(MAX_TRIANGLES * sizeof(triangle));
//
//	int v_count = 0, t_count = 0;
//	char line[256];
//
//	errno_t	e = fopen_s(&input_file, file_name, "r");
//
//	if (!input_file)
//	{
//		free(vertices);
//		free(triangles);
//		return 0;
//	}
//
//	while (fgets(line, sizeof(line), input_file))
//	{
//		if (line[0] == 'v')
//		{
//			if (line[1] == ' ')
//			{
//				if (v_count < MAX_VERTICES)
//				{
//					sscanf_s(line, "v %f %f %f", &vertices[v_count].x, &vertices[v_count].y, &vertices[v_count].z);
//					vertices[v_count].w = 1.0f;
//					v_count++;
//				}
//				else
//				{
//					printf("Maximum number of vertices reached.\n");
//				}
//			}
//		}
//		else if (line[0] == 'f')
//		{
//			if (line[1] == ' ')
//			{
//				if (t_count < MAX_TRIANGLES)
//				{
//					int p1, p2, p3;
//					sscanf_s(line, "f %d %d %d", &p1, &p2, &p3);
//
//					triangles[t_count].v[0] = vertices[p1 - 1];
//					triangles[t_count].v[1] = vertices[p2 - 1];
//					triangles[t_count].v[2] = vertices[p3 - 1];
//
//					// This line is necessary for objects to be rendered correctly, I have no idea why all of the axis need to be flipped, but they do.
//					triangle_multiply_scalar(&triangles[t_count], &triangles[t_count], -1.0f, -1.0f, 1.0f);
//
//					t_count++;
//				}
//				else
//				{
//					printf("Maximum number of triangles reached.\n");
//				}
//			}
//		}
//	}
//
//	qsort(triangles, t_count, sizeof(triangle), compare_triangles);
//
//	for (int i = 0; i < t_count; i++)
//	{
//		empty_mesh->t[i] = triangles[i];
//	}
//	empty_mesh->c = t_count;
//
//
//	fclose(input_file);
//
//	free(vertices);
//	free(triangles);
//
//	return 1;
//}
