#include "state.h"

// Main Loop:
int main(int argc, char* argv[])
{
	state c_state;

	printf("Starting DEMO...\n");

	state_init(&c_state);
	state_setup(&c_state);

	while (!c_state.quit)
	{
		state_process_input(&c_state);
		state_update(&c_state);
		state_render(&c_state);
	}

	state_destroy(&c_state);
	return 0;
}
