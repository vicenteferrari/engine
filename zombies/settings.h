//
// Created by vfs on 2020-09-20.
//

#ifndef TRUNK_SETTINGS_H
#define TRUNK_SETTINGS_H

enum VIDEO_DRIVER {
	DIRECT3D11,
	OPENGL
};

struct Settings {
	VIDEO_DRIVER video_driver;
	int vsync;

	bool fullscreen; // true is borderless fullscreen, false is windowed with decorations
	u32 width;
	u32 height;

	u8 volume;
};

ZOMBIES_RESULT init_settings(Settings *settings);
ZOMBIES_RESULT process_arguments(Settings *settings, int argc, char *argv[]);

#endif //TRUNK_SETTINGS_H
