//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_CAMERA_H
#define ZOMBIES_CAMERA_H

struct Camera {
	dx::XMFLOAT3 pos;
	dx::XMFLOAT3 dir;
	dx::XMFLOAT3 up;
	dx::XMFLOAT3 right;
	dx::XMFLOAT3 world_up;

	float yaw;
	float pitch;

	bool forward;
	bool strafe_left;
	bool backward;
	bool strafe_right;

	float dyaw;
	float dpitch;

	float movement_speed;
	float mouse_sensitivity;
	float fov;

	Camera();
	void input();
	void update(float dt);
	void imgui();
	void reset();
	void recalculate();
	dx::XMMATRIX get_matrix();
//	void rotate(float dx, float dy);
//	void translate(dx::XMFLOAT3 delta);

	static void blend(float alpha, Camera *blended, Camera *previous, Camera *current);
};

#endif
