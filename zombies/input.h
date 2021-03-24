//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_INPUT_H
#define ZOMBIES_INPUT_H

struct RawMouse {
	bool enabled;

	s32 dx;
	s32 dy;
};

void poll_raw_mouse();

struct Mouse {
	bool enabled;

	s32 x;
	s32 y;
};

void poll_mouse();

// TODO: we will want a better struct in the future to represent the keyboard state in a frame.
// keep in mind that the keyboard state that frame n sees was input during frame n-1.
struct Keyboard {
	bool W;
	bool A;
	bool S;
	bool D;
};

void poll_keyboard();

struct VirtualController {
	RawMouse raw_mouse;
	Mouse mouse;
	Keyboard keyboard;
};

void poll_virtual_controller();

//struct MouseButton {
//	s32 half_transition_count;
//	bool ended_down;
//};
//
//struct MouseInput {
//	float start_x;
//	float start_y;
//
//	float min_x;
//	float min_y;
//
//	float max_x;
//	float max_y;
//
//	float end_x;
//	float end_y;
//
//	union {
//		MouseButton buttons[3];
//		struct {
//			MouseButton mouse1;
//			MouseButton mouse2;
//			MouseButton mouse_middle;
//		};
//	};
//};
//
//struct GameButtonState {
//	s32 half_transition_count;
//	bool ended_down;
//};
//
//struct GameControllerInput {
//	bool is_analog;
//
//	float start_x;
//	float start_y;
//
//	float min_x;
//	float min_y;
//
//	float max_x;
//	float max_y;
//
//	float end_x;
//	float end_y;
//
//	union {
//		GameButtonState buttons[6];
//		struct {
//			GameButtonState up;
//			GameButtonState down;
//			GameButtonState left;
//			GameButtonState right;
//			GameButtonState left_shoulder;
//			GameButtonState right_shoulder;
//		};
//	};
//};
//
//struct GameInput {
//	GameControllerInput controllers[4];
//	MouseInput mouse;
//};
//
//void update_raw_mouse(u32 x, u32 y);
//void process_XInput_digital_button(DWORD XInput_button_state, GameButtonState *old_state, DWORD button_bit, GameButtonState *new_state);
//
//void update_input(GameInput *old_input, GameInput *new_input);
//
void process_keyboard_sdl();

#endif
