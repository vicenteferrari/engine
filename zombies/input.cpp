//
// Created by vfs on 5/23/2020.
//

extern const u8 *keyboard_state_sdl;
extern VirtualController virtual_controller;

extern SDL_Event event;

extern bool running;

extern bool relative_mouse;

void poll_raw_mouse() {
	SDL_GetRelativeMouseState(&virtual_controller.raw_mouse.dx, &virtual_controller.raw_mouse.dy);
}

void poll_mouse() {
	SDL_GetMouseState(&virtual_controller.mouse.x, &virtual_controller.mouse.y);
}

void poll_keyboard() {
	if (keyboard_state_sdl[SDL_SCANCODE_W]) {
		virtual_controller.keyboard.W = true;
	} else {
		virtual_controller.keyboard.W = false;
	}

	if (keyboard_state_sdl[SDL_SCANCODE_A]) {
		virtual_controller.keyboard.A = true;
	} else {
		virtual_controller.keyboard.A = false;
	}

	if (keyboard_state_sdl[SDL_SCANCODE_S]) {
		virtual_controller.keyboard.S = true;
	} else {
		virtual_controller.keyboard.S = false;
	}

	if (keyboard_state_sdl[SDL_SCANCODE_D]) {
		virtual_controller.keyboard.D = true;
	} else {
		virtual_controller.keyboard.D = false;
	}
}

void poll_virtual_controller() {
	if (relative_mouse) {
		poll_raw_mouse();
	} else {
		poll_mouse();
	}
	poll_keyboard();
}

//void update_raw_mouse(s32 x, s32 y) {
//
//}
//
//void process_XInput_digital_button(DWORD XInput_button_state, GameButtonState *old_state, DWORD button_bit, GameButtonState *new_state) {
//	new_state->ended_down = (XInput_button_state & button_bit) == button_bit;
//	new_state->half_transition_count = old_state->ended_down != new_state->ended_down ? 1 : 0;
//}
//
//void update_input(GameInput *old_input, GameInput *new_input) {
//	// poll controller input
//	for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex) {
//
//		GameControllerInput *oldController = &old_input->controllers[controllerIndex];
//		GameControllerInput *newController = &new_input->controllers[controllerIndex];
//
//		XINPUT_STATE controllerState;
//		if (XInputGetState(controllerIndex, &controllerState) == 0L) {
//			XINPUT_GAMEPAD *pad = &controllerState.Gamepad;
//
//
//			bool up = pad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
//			bool down = pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
//			bool left = pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
//			bool right = pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
//			bool start = pad->wButtons & XINPUT_GAMEPAD_START;
//			bool back = pad->wButtons & XINPUT_GAMEPAD_BACK;
//			bool leftThumb = pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
//			bool rightThumb = pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
//			bool leftShoulder = pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
//			bool rightShoulder = pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
//			bool aButton = pad->wButtons & XINPUT_GAMEPAD_A;
//			bool bButton = pad->wButtons & XINPUT_GAMEPAD_B;
//			bool xButton = pad->wButtons & XINPUT_GAMEPAD_X;
//			bool ybutton = pad->wButtons & XINPUT_GAMEPAD_Y;
//
//			int16_t leftStickX = pad->sThumbLX;
//			int16_t leftStickY = pad->sThumbLY;
//			int16_t rightStickX = pad->sThumbRX;
//			int16_t rightStickY = pad->sThumbRY;
//
//			newController->start_x = oldController->end_x;
//			newController->start_y = oldController->end_x;
//
//			float leftStickXNorm;
//			if (leftStickX > 0) {
//				leftStickXNorm = (float) leftStickX / 32767.0f;
//			} else {
//				leftStickXNorm = (float) leftStickX / 32768.0f;
//			}
//
//			float leftStickYNorm;
//			if (leftStickY > 0) {
//				leftStickYNorm = (float) leftStickY / 32767.0f;
//			} else {
//				leftStickYNorm = (float) leftStickY / 32768.0f;
//			}
//
//			float rightStickXNorm;
//			if (rightStickX > 0) {
//				rightStickXNorm = (float) rightStickX / 32767.0f;
//			} else {
//				rightStickXNorm = (float) rightStickX / 32768.0f;
//			}
//
//			float rightStickYNorm;
//			if (rightStickY > 0) {
//				rightStickYNorm = (float) rightStickY / 32767.0f;
//			} else {
//				rightStickYNorm = (float) rightStickY / 32768.0f;
//			}
//
//			//newController->minX = oldController->maxX = newController->endX = ;
//
//			process_XInput_digital_button(pad->wButtons, &oldController->up, XINPUT_GAMEPAD_DPAD_UP, &newController->up);
//			process_XInput_digital_button(pad->wButtons, &oldController->down, XINPUT_GAMEPAD_DPAD_DOWN, &newController->down);
//			process_XInput_digital_button(pad->wButtons, &oldController->left, XINPUT_GAMEPAD_DPAD_LEFT, &newController->left);
//			process_XInput_digital_button(pad->wButtons, &oldController->right, XINPUT_GAMEPAD_DPAD_RIGHT, &newController->right);
//		}
//	}
//
//	GameInput *temp = new_input;
//	new_input = old_input;
//	old_input = temp;
//}
