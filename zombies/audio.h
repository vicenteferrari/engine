//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_SOUND_H
#define ZOMBIES_SOUND_H

struct game_sound_output {
	int samplesPerSecond;
	int hz;
	int16_t volume;
	uint32_t runningSampleIndex;
	int period;
	int bytesPerSample;
	int bufferSize;
};

ZOMBIES_RESULT init_sound(HWND window, int32_t samplesPerSec, int32_t bufferSize);
//void update_sound();
//void fill_sound_buffer(game_sound_output *soundOutput, DWORD byteToLock, DWORD bytesToWrite);

#endif
