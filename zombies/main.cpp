//
// Created by vfs on 5/23/2020.
//

// from stdlib
#include <initguid.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <cassert>

// from windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincodec.h>
#include <d3d11.h>
#include <d2d1.h>
#include <xinput.h>
#include <dwrite.h>
#include <d3dcompiler.h>

// from extern
#include <SDL2/SDL.h>
//#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <imgui/imgui.cpp>
#include <imgui/imgui_demo.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_widgets.cpp>
#include <imgui/implot.cpp>
#include <imgui/implot_items.cpp>
#include <DirectXMath/DirectXMath.h> // appareantly this needs to be included after SDL.h becasue of conflict between
									 // cpuid.h and intrin.h...
namespace dx = DirectX;

// from common
#include "common.h"
#include "maths.h"
#include "memory.h"
#include "n_point_light.h"
#include "n_zombie.h"
#include "network.h"
#include "ring_buffer.h"

// to compile
#include "imgui_impl_dx11.h"
//#include "imgui_impl_win32.h"
//#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
//#include "audio.h"
#include "bindables_d3d11.h"
#include "camera.h"
#include "client.h"
#include "game.h"
#include "graphics_d3d11.h"
#include "graphics_opengl.h"
#include "input.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "settings.h"
#include "shader.h"
#include "model.h"
#include "window_sdl.h"
#include "game_state.h"
#include "zombie.h"
#include "point_light.h"


#include "imgui_impl_dx11.cpp"
//#include "imgui_impl_win32.cpp"
//#include "imgui_impl_opengl3.cpp"
#include "imgui_impl_sdl.cpp"
//#include "audio.cpp"
#include "bindables_d3d11.cpp"
#include "camera.cpp"
#include "client.cpp"
#include "game.cpp"
#include "graphics_d3d11.cpp"
#include "graphics_opengl.cpp"
#include "input.cpp"
#include "texture.cpp"
#include "material.cpp"
#include "mesh.cpp"
#include "model.cpp"
#include "point_light.cpp"
#include "settings.cpp"
#include "shader.cpp"
#include "game_state.cpp"
#include "window_sdl.cpp"
#include "zombie.cpp"


// global memory
Window_sdl window_sdl;
SDL_Event event;

const u8 *keyboard_state_sdl;
VirtualController virtual_controller;

Memory memory;
Client client;

MeshManager mesh_manager;
ShaderManager shader_manager;
TextureManager texture_manager;

u64 mesh_id;
u64 shader_id;
u64 material_id;
u64 texture_id;
u64 model_id;
u64 entity_id;
u64 sound_id;
u64 music_id;

GameState *previous_state;
GameState *current_state;
GameState *render_state;

IDXGISwapChain *swap_chain;
ID3D11Device *device;
ID3D11DeviceContext *context;
ID3D11RenderTargetView *target;
ID3D11DepthStencilView *depth_view;
ID2D1RenderTarget *target2d;

bool running = true;
bool relative_mouse = false;

double t = 0.0;
double gt = 0.0;
float dt = DELTATIME;
float k = 1.0f;
u64 frame = 0;
float frame_time = 0.0f;
float fps = 0.0f;
float accumulator = 0.0f;
u64 qpc_freq;

Settings settings;

RingBuffer ring_time;
RingBuffer ring_acc;
RingBuffer ring_ft;
RingBuffer ring_ut;
RingBuffer ring_rt;
RingBuffer ring_mouse_deltas;

int main(int argc, char *argv[]) {

	setbuf(stdout, NULL);

#if ZOMBIES_DEBUG
	printf("Starting ZOMBIES DEBUG\n");
#else
	printf("Starting ZOMBIES\n");
#endif

	if (init(argc, argv) == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;

	float *data_time_memory = (float *) arena_alloc(&memory.temp_memory_arena, SAMPLES_COUNT * sizeof(float));
	if(rb_init(&ring_time, data_time_memory, SAMPLES_COUNT) == ZOMBIES_ERROR)
		assert(false);

	float *data_acc_memory = (float *) arena_alloc(&memory.temp_memory_arena, SAMPLES_COUNT * sizeof(float));
	if (rb_init(&ring_acc, data_acc_memory, SAMPLES_COUNT) == ZOMBIES_ERROR)
		assert(false);

	float *data_ft_memory = (float *) arena_alloc(&memory.temp_memory_arena, SAMPLES_COUNT * sizeof(float));
	if (rb_init(&ring_ft, data_ft_memory, SAMPLES_COUNT) == ZOMBIES_ERROR)
		assert(false);

	float *data_ut_memory = (float *) arena_alloc(&memory.temp_memory_arena, SAMPLES_COUNT * sizeof(float));
	if (rb_init(&ring_ut, data_ut_memory, SAMPLES_COUNT) == ZOMBIES_ERROR)
		assert(false);

	float *data_rt_memory = (float *) arena_alloc(&memory.temp_memory_arena, SAMPLES_COUNT * sizeof(float));
	if (rb_init(&ring_rt, data_rt_memory, SAMPLES_COUNT) == ZOMBIES_ERROR)
		assert(false);

	float *data_mouse_deltas_memory = (float *) arena_alloc(&memory.temp_memory_arena, SAMPLES_COUNT * sizeof(float));
	if (rb_init(&ring_mouse_deltas, data_mouse_deltas_memory, SAMPLES_COUNT) == ZOMBIES_ERROR)
		assert(false);

	u64 current_counter = SDL_GetPerformanceCounter();

	// main game loop

	while (running) {

		event_loop_sdl();

		// begin timing

		u64 new_counter = SDL_GetPerformanceCounter();

		u64 frame_counter = new_counter - current_counter;

		current_counter = new_counter;

		frame_time = (float) ((double) frame_counter / (double) qpc_freq);
		if (frame_time > 0.25f)
			frame_time = 0.25f;

		t += frame_time;

		accumulator += frame_time * k;

		fps = 1.0f / frame_time;

		if (frame % 100 == 0) {
			shader_manager.hot_reloads();
		}

		// IMPORTANT:
		// we want the main loop's input function to poll the hardware and set the virtual controller state
		// then each game object's input function polls the virtual controller and sets its own state
		// then each game object's time dependant update function does physics according to that object's state
		input();

		u64 old_update_counter = SDL_GetPerformanceCounter();
		while (accumulator >= dt * k) {
			memcpy(previous_state, current_state, sizeof(GameState));

			update(dt * k);

			gt += dt * k;
			accumulator -= dt * k;
		}
#ifdef ZOMBIES_DEBUG
		rb_push_back(&ring_acc, accumulator);
#endif

		memcpy(render_state, current_state, sizeof(GameState));
		const float alpha = accumulator / dt;
		GameState::blend(alpha, render_state, previous_state, current_state);

		u64 new_update_counter = SDL_GetPerformanceCounter();
		u64 update_counter = new_update_counter - old_update_counter;
		float update_time = (float) ((double) update_counter / (double) qpc_freq);

#ifdef ZOMBIES_DEBUG
		u64 old_render_counter = SDL_GetPerformanceCounter();
		render();
		u64 new_render_counter = SDL_GetPerformanceCounter();
		u64 render_counter = new_render_counter - old_render_counter;
		float render_time = (float) ((double) render_counter / (double) qpc_freq);
#else
		render();
#endif



#ifdef ZOMBIES_DEBUG
		rb_push_back(&ring_time, t);
		rb_push_back(&ring_ft, frame_time);
		rb_push_back(&ring_rt, render_time);
		rb_push_back(&ring_ut, update_time);
#endif

		//client.connection_timer += frame_time;

		//printf("%.8f s/f, %.4f f/s \n", frame_time, fps);


		++frame;
	}

	// destroys

	//client.destroy();

	ImGui_ImplDX11_Shutdown();
//	ImGui_ImplWin32_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
