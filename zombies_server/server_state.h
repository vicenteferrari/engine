//
// Created by vfs on 30-Jun-20.
//

#ifndef ZOMBIES_SERVER_STATE_H
#define ZOMBIES_SERVER_STATE_H

struct NPointLight;
struct NZombie;

struct ServerState {
	float t1;
	float t2;
	float t3;

	// lights
	u64 lights_count;
	u64 lights_curr;
	NPointLight *lights;

	// entities
	u64 zombies_count;
	u64 zombies_curr;
	NZombie *zombies;

	ServerState();
	void update(float dt);

	static void blend(float alpha, ServerState *blended, ServerState *previous, ServerState *current);
};

#endif //ZOMBIES_SERVER_STATE_H
