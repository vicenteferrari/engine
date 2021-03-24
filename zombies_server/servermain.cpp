//
// Created by vfs on 5/23/2020.
//

// from stdlib
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <new>

// from windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// from extern
/* #include "raknet/RakPeerInterface.h" */
/* #include "raknet/MessageIdentifiers.h" */
/* #include "raknet/BitStream.h" */
/* #include "raknet/RakNetTypes.h" */
/* #include "raknet/ConnectionGraph2.h" */

// from common
#include "common.h"
#include "types.h"
#include "memory.h"
#include "maths.h"
#include "n_point_light.h"
#include "n_zombie.h"

// to compile
#include "game.h"
#include "server.h"
#include "server_state.h"

#include "game.cpp"
#include "server.cpp"
#include "server_state.cpp"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

Memory memory;
Server server;

ServerState *state;

bool running = true;

double t = 0.0;
double gt = 0.0f;
float dt = DELTATIME;
float k = 1.0f;
u64 frame = 0;

float frame_time = 0.0f;
float fps = 0.0f;
float accumulator = 0.0f;

float update_interval = 1.0f / 20.0f;
float tick_accumulator = 0.0f;

ZOMBIES_RESULT init();
u64 get_qpc();
u64 get_qpc_freq();

int main(int argc, char *argv[]) {

	setbuf(stdout, NULL);

#if ZOMBIES_DEBUG
	printf("Starting ZOMBIES DEBUG\n");
#else
	printf("Starting ZOMBIES\n");
#endif

	if (init() == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;

	u64 current_counter = get_qpc();

	// main game loop

	while (running) {
		u64 new_counter = get_qpc();

		u64 frame_counter = new_counter - current_counter;

		current_counter = new_counter;

		frame_time = (float) ((double) frame_counter / (double) get_qpc_freq());
		if (frame_time > 0.25f)
			frame_time = 0.25f;
		
		t += frame_time;

		accumulator += frame_time * k;
		tick_accumulator += frame_time;

		fps = 1.0f / frame_time;

		while (accumulator >= dt * k) {
			update(dt * k);

			gt = dt * k;
			accumulator -= dt * k;
		}

		if (tick_accumulator >= update_interval) {
			//printf("%.8f s/f, %.4f f/s \n", frame_time, fps);
			printf("Time: %d s\n", (int) t);

			server.send();

			tick_accumulator -= update_interval;
		}

		++frame;
	}

	return 0;
}

ZOMBIES_RESULT init() {
	// init game globals

	//client = Client(Address(127, 0, 0, 1, DEFAULT_PORT));
	//printf("init networking done\n");

	if (init_memory(&memory) == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished initializing memory arena");

	assert(memory.state_memory_size > sizeof(ServerState) * 3);

//	previous_state = (GameState *) memory.state_memory;
//	current_state = (GameState *) (((GameState *) memory.state_memory) + 1);
//	render_state = (GameState *) (((GameState *) memory.state_memory) + 2);

	state = new(memory.state_memory) ServerState();

//	if (init_state(previous_state, &memory) == ZOMBIES_ERROR)
//		return ZOMBIES_ERROR;
//
//	if (init_state(current_state, &memory) == ZOMBIES_ERROR)
//		return ZOMBIES_ERROR;
//
//	if(init_state(render_state, &memory) == ZOMBIES_ERROR)
//		return ZOMBIES_ERROR;

	LOG_INFO("Finished initializing game state");

	return ZOMBIES_SUCCESS;
}

u64 get_qpc() {
	LARGE_INTEGER qpc;
	QueryPerformanceCounter(&qpc);

	return qpc.QuadPart;
}

u64 get_qpc_freq() {
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	return freq.QuadPart;
}
