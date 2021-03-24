//
// Created by vfs on 2020-09-19.
//

extern Window_sdl window_sdl;
extern SDL_Event event;

extern const u8 *keyboard_state_sdl;

extern Settings settings;

extern bool relative_mouse;

ZOMBIES_RESULT init_window_sdl() {
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
//	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
//
//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

//	glEnable(GL_MULTISAMPLE);

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
	SDL_SetHint(SDL_HINT_RENDER_DIRECT3D11_DEBUG, "1");

	window_sdl.window = SDL_CreateWindow("Zombies",
										 SDL_WINDOWPOS_CENTERED,
										 SDL_WINDOWPOS_CENTERED,
										 settings.width,
										 settings.height,
										 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_SysWMinfo wm_info;
	SDL_VERSION(&wm_info.version);
	SDL_GetWindowWMInfo(window_sdl.window, &wm_info);
	window_sdl.win32_handle = wm_info.info.win.window;

	qpc_freq = SDL_GetPerformanceFrequency();
	keyboard_state_sdl = SDL_GetKeyboardState(NULL);

	toggle_relative_mouse();

//	context = SDL_GL_CreateContext(window_win32);
//	SDL_GL_MakeCurrent(window_sdl.window, context);

//	glewInit();
//
//	glEnable(GL_DEPTH_TEST);
//
//	glEnable(GL_STENCIL_TEST);
//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//	glViewport(0, 0, width, height);
//
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImPlot::CreateContext();

	ImGui_ImplSDL2_InitForD3D(window_sdl.window);
//	ImGui_ImplSDL2_InitForOpenGL(window_win32, context);
//	ImGui_ImplOpenGL3_Init("#version 330");


	stbi_set_flip_vertically_on_load(true);

	return ZOMBIES_SUCCESS;
}

void process_keyboard_sdl() {
	switch (event.key.keysym.sym) {
		case SDLK_ESCAPE: {
			running = false;
			break;
		}

		case SDLK_F1: {
			toggle_relative_mouse();
			break;
		}
	}
}

void event_loop_sdl() {
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type) {
			case SDL_KEYDOWN: {
				process_keyboard_sdl();
				break;
			}
		}
	}
}

void toggle_relative_mouse() {
	if (relative_mouse) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		relative_mouse = false;
	} else {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		relative_mouse = true;
	}

	// sdl apparently accumulates all the movement of the mouse when it's not relative
	// and then when you toggle it gives it all at once in one poll
	poll_raw_mouse();
	virtual_controller.raw_mouse.dx = 0;
	virtual_controller.raw_mouse.dy = 0;
}

float get_ar() {
	return (float) settings.height / (float) settings.width;
}
