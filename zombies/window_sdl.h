//
// Created by vfs on 2020-09-19.
//

#ifndef ZOMBIES_MAKE_WINDOW_SDL_H
#define ZOMBIES_MAKE_WINDOW_SDL_H

struct Window_sdl {
	SDL_Window *window;
	HWND win32_handle; // d3d11 needs it
};

ZOMBIES_RESULT init_window_sdl();
void event_loop_sdl();
void toggle_relative_mouse();
float get_ar();

#endif //ZOMBIES_MAKE_WINDOW_SDL_H
