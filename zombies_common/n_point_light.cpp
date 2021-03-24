//
// Created by vfs on 29-Jun-20.
//

NPointLight::NPointLight() {
	this->position = {0.0f, 0.0f, -2.0f};
	this->velocity = {0.0f, 0.0f, 0.0f};

	this->orientation = { 0.0f, 0.0f, 0.0f};
	this->angular = {0.0f, 1.0f, 0.0f};
}

void NPointLight::update(float dt) {

}


void NPointLight::blend(float alpha, NPointLight *blended, NPointLight *previous, NPointLight *current) {
	blended->position.x = current->position.x * alpha + previous->position.x * (1.0f - alpha);
	blended->position.y = current->position.y * alpha + previous->position.y * (1.0f - alpha);
	blended->position.z = current->position.z * alpha + previous->position.z * (1.0f - alpha);

	blended->velocity.x = current->velocity.x * alpha + previous->velocity.x * (1.0f - alpha);
	blended->velocity.y = current->velocity.y * alpha + previous->velocity.y * (1.0f - alpha);
	blended->velocity.z = current->velocity.z * alpha + previous->velocity.z * (1.0f - alpha);

	blended->orientation.pitch = current->orientation.pitch * alpha + previous->orientation.pitch * (1.0f - alpha);
	blended->orientation.yaw = current->orientation.yaw * alpha + previous->orientation.yaw * (1.0f - alpha);
	blended->orientation.roll = current->orientation.roll * alpha + previous->orientation.roll * (1.0f - alpha);

	blended->angular.roll = current->angular.roll * alpha + previous->angular.roll * (1.0f - alpha);
	blended->angular.pitch = current->angular.pitch * alpha + previous->angular.pitch * (1.0f - alpha);
	blended->angular.yaw = current->angular.yaw * alpha + previous->angular.yaw * (1.0f - alpha);
}
