//
// Created by vfs on 5/23/2020.
//

extern Memory memory;
extern double t;

GameState::GameState() {
	t1 = 0.0f;
	t2 = 0.0f;
	t3 = 0.0f;

	camera = Camera();

	lights_count = 1;
	lights_curr = 0;
	lights = (PointLight *) arena_alloc(&memory.temp_memory_arena, lights_count * sizeof(PointLight));

	zombies_count = 1;
	zombies_curr = 0;
	zombies = (Zombie *) arena_alloc(&memory.temp_memory_arena, zombies_count * sizeof(Zombie));

	for (u64 i = 0; i < lights_count; ++i) {
		new(lights + lights_curr++) PointLight(this);
		//init_pointlight(&lights[lights_curr++], this, 0.0f, 0.0f, -2.0f);
	}

	for (u64 i = 0; i < zombies_count; ++i) {
		new(zombies + zombies_curr++) Zombie(this);
		//init_zombie(&zombies[zombies_curr++], this);
	}
}

void GameState::input() {
	camera.input();
}

void GameState::update(float dt) {
	t1 = (float) ((sin(t / 10.0) + 1.0) / 2.0);
	t2 = (float) ((sin((t / 10.0) + M_PI) + 1.0) / 2.0);
	t3 = (float) ((sin((t / 10.0) + M_PI / 2) + 1.0) / 2.0);

	camera.update(dt);

	// right now we have to update the gpu buffer of the point lights every frame
	for (u64 i = 0; i < lights_curr; ++i) {
		lights[i].update();
	}

	for (u64 i = 0; i < zombies_curr; ++i) {
		zombies[i].update(dt);
	}
}

void GameState::blend(float alpha, GameState *blended, GameState *previous, GameState *current) {
	blended->t1 = current->t1 * alpha + previous->t1 * (1.0f - alpha);
	blended->t2 = current->t2* alpha + previous->t2 * (1.0f - alpha);
	blended->t3 = current->t3 * alpha + previous->t3 * (1.0f - alpha);

	//Camera::blend(alpha, &blended->camera, &previous->camera, &current->camera);

	for (u64 i = 0; i < current->lights_curr; ++i) {
		blended->lights[i] = current->lights[i];
	}

	for (u64 i = 0; i < current->zombies_curr; ++i) {
		Zombie::blend(alpha, &blended->zombies[i], &previous->zombies[i], &current->zombies[i]);
	}
}
