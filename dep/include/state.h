#ifndef STATE_H
#define STATE_H

#include "config.h"
#include "mesh.h"
#include "triangle.h"
#include "vertex.h"
#include "quaternion.h"
#include "matrix.h"

// Struct to manage SDL components and game state:
typedef struct state
{
	// SDL Objects:
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	// Bool States:
	int quit;

	// State Time Management:
	int last_frame_time; // Unit: (ms)
	f32 f_delta_time; // Unit: (seconds)

	// Objects:
	mesh m_map;
	mesh m_player;
	quaternion q[10];
	matrix projection_matrix, camera_matrix;
	vertex player_position, camera_position, light_position, light_dir, look_dir, up;


} state;

// Initialize State Struct:
void state_init(state* state);

// Destroy State Window, Renderer, and Texture:
void state_destroy(state* state);

// Function to handle timing:
void state_delay(state* state);

// Setup Tasks:
void state_setup(state* state);

// Receive User Input and Process:
void state_process_input(state* state);

// Update State Objects:
void state_update(state* state);

// Render State objects:
void state_render(state* state);

void state_move_player(state* state, f32 x, f32 y, f32 z);

#endif
