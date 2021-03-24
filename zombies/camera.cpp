//
// Created by vfs on 5/23/2020.
//

extern VirtualController virtual_controller;

extern Settings settings;

Camera::Camera() {
	reset();
}

void Camera::input() {
	if (virtual_controller.keyboard.W) {
		this->forward = true;
	} else {
		this->forward = false;
	}

	if (virtual_controller.keyboard.A) {
		this->strafe_left = true;
	} else {
		this->strafe_left = false;
	}

	if (virtual_controller.keyboard.S) {
		this->backward = true;
	} else {
		this->backward = false;
	}

	if (virtual_controller.keyboard.D) {
		this->strafe_right = true;
	} else {
		this->strafe_right = false;
	}

	this->dyaw = (float) virtual_controller.raw_mouse.dx;
	this->dpitch = (float) virtual_controller.raw_mouse.dy;

	yaw -= dyaw * mouse_sensitivity;
	pitch += dpitch * mouse_sensitivity;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	recalculate();
}

void Camera::update(float dt) {
	if (forward) {
		dx::XMVECTOR temp = dx::XMLoadFloat3(&pos);
		temp -= dx::XMVector3Normalize(dx::XMLoadFloat3(&dir)) * movement_speed * dt;
		dx::XMStoreFloat3(&pos, temp);
	}

	if (strafe_left) {
		dx::XMVECTOR temp = dx::XMLoadFloat3(&pos);
		temp -= dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMLoadFloat3(&dir), dx::XMLoadFloat3(&up))) * movement_speed * dt;
		dx::XMStoreFloat3(&pos, temp);
	}

	if (backward) {
		dx::XMVECTOR temp = dx::XMLoadFloat3(&pos);
		temp -= -dx::XMVector3Normalize(dx::XMLoadFloat3(&dir)) * movement_speed * dt;
		dx::XMStoreFloat3(&pos, temp);
	}

	if (strafe_right) {
		dx::XMVECTOR temp = dx::XMLoadFloat3(&pos);
		temp -= -dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMLoadFloat3(&dir), dx::XMLoadFloat3(&up))) * movement_speed * dt;
		dx::XMStoreFloat3(&pos, temp);
	}

	pos.y = 0.0f;
}

void Camera::imgui() {
	if (ImGui::Begin("Camera")) {
		ImGui::SliderFloat3("Position", &pos.x, -20.0f, 20.0f);
		ImGui::Text("Orientation");
		ImGui::SliderFloat("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset")) {
			reset();
		}
	}
	ImGui::End();
}

void Camera::reset() {
	pos = {0.0f, 0.0f, -10.0f};
	dir = {0.0f, 0.0f, 1.0f};
	up = {0.0f, 1.0f, 0.0f};
	world_up = {0.0f, 1.0f, 0.0f};

	yaw = -90.0f;
	pitch = 0.0f;

	movement_speed = 12.0f;
	mouse_sensitivity = 0.025f;
	fov = 90.0f;

	recalculate();
}

void Camera::recalculate() {
	dx::XMFLOAT3 new_dir;
	new_dir.x = cosf(dx::XMConvertToRadians(yaw)) * cosf(dx::XMConvertToRadians(pitch));
	new_dir.y = sinf(dx::XMConvertToRadians(pitch));
	new_dir.z = sinf(dx::XMConvertToRadians(yaw)) * cosf(dx::XMConvertToRadians(pitch));

	dir = new_dir;

	dx::XMStoreFloat3(&right, dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMLoadFloat3(&dir), dx::XMLoadFloat3(&world_up))));
	dx::XMStoreFloat3(&up, dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMLoadFloat3(&right), dx::XMLoadFloat3(&dir))));
}

dx::XMMATRIX Camera::get_matrix() {

	return dx::XMMatrixLookAtLH(dx::XMLoadFloat3(&pos), dx::XMLoadFloat3(&pos) - dx::XMLoadFloat3(&dir), dx::XMLoadFloat3(&up));


	// const auto pos = dx::XMVector3Transform(
	// 		dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
	// 		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	// );
	// return dx::XMMatrixLookAtLH(
	// 		pos, dx::XMVectorZero(),
	// 		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	// ) * dx::XMMatrixRotationRollPitchYaw(
	// 		pitch, -yaw, roll
	// );
}

//void Camera::rotate(float dx, float dy) {
//	float a = M_PI / 180.0f;
//
//	float angle_to_move_x = dx * a;
//	float t_yaw = fmod(yaw + angle_to_move_x, M_PI * 2);
//	yaw = t_yaw;
//
//	float angle_to_move_y = dy * a;
//	float t_pitch = fclamp(pitch + angle_to_move_y, M_PI / 2.0f * 0.9f, -M_PI / 2.0f * 0.9f);
//	pitch = t_pitch;
//}
//
//void Camera::translate(dx::XMFLOAT3 delta) {
//	dx::XMStoreFloat3(&delta,
//		dx::XMVector3Transform(dx::XMLoadFloat3(&delta),
//		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) * dx::XMMatrixScaling(movement_speed, movement_speed, movement_speed))
//	);
//
//	pos.x += delta.x;
//	pos.y += delta.y;
//	pos.z += delta.z;
//}

void Camera::blend(float alpha, Camera *blended, Camera *previous, Camera *current) {
	Vec3::blend(alpha, (Vec3 *) &blended->pos, (Vec3 *) &previous->pos, (Vec3 *) &current->pos);

	blended->pitch = current->pitch * alpha + previous->pitch * (1.0f - alpha);
	blended->yaw = current->yaw * alpha + previous->yaw * (1.0f - alpha);
}
