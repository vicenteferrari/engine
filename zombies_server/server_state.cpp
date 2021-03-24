//
// Created by vfs on 30-Jun-20.
//

extern Memory memory;

ServerState::ServerState() {
	t1 = 0.0f;
	t2 = 0.0f;
	t3 = 0.0f;

	lights_count = 1;
	lights_curr = 0;
	lights = (NPointLight *) arena_alloc(&memory.temp_memory_arena, lights_count * sizeof(NPointLight));

	zombies_count = 1;
	zombies_curr = 0;
	zombies = (NZombie *) arena_alloc(&memory.temp_memory_arena, zombies_count * sizeof(NZombie));

	for (u64 i = 0; i < lights_count; ++i) {
		new(lights + lights_curr++) NPointLight();
	}

	for (u64 i = 0; i < zombies_count; ++i) {
		new(zombies + zombies_curr++) NZombie();
	}
}

void ServerState::update(float dt) {
	for (u64 i = 0; i < zombies_curr; ++i) {
		zombies[i].update(dt);
	}
}
