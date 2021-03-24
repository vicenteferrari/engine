//
// Created by vfs on 29-Jun-20.
//

NZombie::NZombie() {
	position = {0.0f, 0.0f, 0.0f};
	velocity = {0.0f, 0.0f, 0.0f};

	orientation = { 0.0f, 0.0f, 0.0f};
	angular = {0.0f, 1.0f, 0.0f};
}

void NZombie::update(float dt) {
	position = position + velocity * dt;

	orientation = orientation + angular * dt;
	orientation.x = fmod(orientation.x, M_PI * 2);
	orientation.y = fmod(orientation.y, M_PI * 2);
	orientation.z = fmod(orientation.z, M_PI * 2);
}

void NZombie::blend(float alpha, NZombie *blended, NZombie *previous, NZombie *current) {
	Vec3::blend(alpha, &blended->position, &previous->position, &current->position);
	Vec3::blend(alpha, &blended->velocity, &previous->velocity, &current->velocity);

	Vec3::blend(alpha, &blended->orientation, &previous->orientation, &current->orientation);
	Vec3::blend(alpha, &blended->angular, &previous->angular, &current->angular);
}
