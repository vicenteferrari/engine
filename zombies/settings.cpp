//
// Created by vfs on 2020-09-20.
//

ZOMBIES_RESULT init_settings(Settings *settings) {
	settings->video_driver = DIRECT3D11;
	settings->vsync = 0;

	settings->fullscreen = false;
	settings->width = 1280;
	settings->height = 720;

	settings->volume = UINT8_MAX;

	return ZOMBIES_SUCCESS;
}

ZOMBIES_RESULT process_arguments(Settings *settings, int argc, char *argv[]) {
	if (argc == 1)
		return ZOMBIES_SUCCESS;

	if (argc > 1) {
		for (int i = 0; i < argc; ++i) {
			if (strcmp(argv[i], "-opengl") == 0)
				settings->video_driver = OPENGL;
		}
	}

	return ZOMBIES_SUCCESS;
}
