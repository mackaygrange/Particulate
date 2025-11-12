#include "state.h"

void state_init(state* state)
{
	ASSERT(!SDL_Init(SDL_INIT_VIDEO), "SDL initialization failed: %s\n", SDL_GetError());

	state->window = SDL_CreateWindow("DEMO", SDL_WINDOWPOS_CENTERED_DISPLAY(MAIN_DISPLAY), SDL_WINDOWPOS_CENTERED_DISPLAY(MAIN_DISPLAY), SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);
	ASSERT(state->window, "Window initialization failed: %s\n", SDL_GetError());

	state->renderer = SDL_CreateRenderer(state->window, -1, RENDERER_FLAGS);
	ASSERT(state->renderer, "Renderer initialization failed: %s\n", SDL_GetError());

	state->texture = SDL_CreateTexture(state->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	ASSERT(state->texture, "Texture initialization failed: %s\n", SDL_GetError());

	state->quit = false;

	state->f_delta_time = TARGET_FRAME_TIME;
	state->last_frame_time = SDL_GetTicks();

	state->m_map.c = 0;
    state->m_map.flag = 'm';

	state->m_player.c = 0;
    state->m_player.flag = 'p';
}

void state_destroy(state* state)
{
	SDL_DestroyTexture(state->texture);
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);
	SDL_Quit();
}

void state_delay(state* state) 
{
	ui32 frame_end_time = state->last_frame_time + (int)TARGET_FRAME_TIME;
	ui32 current_time;

	while ((current_time = SDL_GetTicks()) < frame_end_time) {
		if (frame_end_time - current_time > 1) {
			SDL_Delay(1);
		}
	}
	state->f_delta_time = (current_time - state->last_frame_time) / 1000.0f;
	state->last_frame_time = current_time;
}

void state_setup(state* state)
{
	vertex_set(&state->player_position, 0.0f, 0.0f, 0.0f, 1.0f);
	vertex_set(&state->camera_position, 0.0f, 0.0f, 25.0f, 1.0f);
    vertex_set(&state->light_position, 0.0f, 0.0f, 25.0f, 1.0f);
	vertex_set(&state->look_dir, 0.0f, 0.0f, -1.0f, 0.0f);
	vertex_set(&state->light_dir, 0.0f, 0.0f, 1.0f, 0.0f);
	vertex_set(&state->up, 0.0f, 1.0f, 0.0f, 0.0f);

	matrix_set_projection(&state->projection_matrix);

	matrix_set_camera(&state->camera_matrix, &state->camera_position, &state->look_dir, &state->up);

	for (int c = -5; c < 5; c++)
	{
		for (int r = -5; r < 5; r++)
		{
			mesh_add_tile(&state->m_map, 0.0f + (f32)r, 0.0f + (f32)c, 0.0f);
			mesh_add_tile(&state->m_map, 0.0f + (f32)r, 0.0f + (f32)c, 1.0f);
			mesh_add_tile(&state->m_map, 0.0f + (f32)r, 0.0f + (f32)c, 2.0f);
		}
	}

	mesh_add_tile(&state->m_player, -0.5f, -0.5f, 0.0f);

}

void state_move_player(state* state, f32 x, f32 y, f32 z)
{
	// Move player_position, camera, and light_source:
	state->player_position.x += x;
	state->camera_position.x += x;

	state->player_position.y += y;
	state->camera_position.y += y;

	state->player_position.z += z;
	state->camera_position.z += z;
	
	// Move player mesh:
	for (int t = 0; t < state->m_player.c; t++)
	{
		for (int v = 0; v < 3; v++)
		{
			state->m_player.t[t].v[v].x += x;
			state->m_player.t[t].v[v].y += y;
			state->m_player.t[t].v[v].z += z;
		}
	}
}

void state_process_input(state* state)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			state->quit = true;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				state->quit = true;
				break;
			}
			if (event.key.keysym.sym == SDLK_w)
			{
				state_move_player(state, 0.0f, state->f_delta_time * 5.0f, 0.0f);
				break;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				state_move_player(state, 0.0f, state->f_delta_time * -5.0f, 0.0f);
				break;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				state_move_player(state, state->f_delta_time * -5.0f, 0.0f, 0.0f);
				break;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				state_move_player(state, state->f_delta_time * 5.0f, 0.0f, 0.0f);
				break;
			}
			break;
		}
	}
}

void state_update(state* state)
{
	// Delay Until Target Frame Time:
	state_delay(state);

	matrix_set_camera(&state->camera_matrix, &state->camera_position, &state->player_position, &state->up);

	state->last_frame_time = SDL_GetTicks();
}

void state_render(state* state)
{
	// Clear Frame:
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
	SDL_RenderClear(state->renderer);

	mesh_render(state->renderer, &state->m_map, &state->projection_matrix, &state->camera_matrix, &state->light_dir);
	mesh_render(state->renderer, &state->m_player, &state->projection_matrix, &state->camera_matrix, &state->light_dir);

	// Present Frame:
	SDL_RenderPresent(state->renderer);
}
