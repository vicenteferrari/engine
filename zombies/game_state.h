//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_GAME_STATE_H
#define ZOMBIES_GAME_STATE_H

struct PointLight;
struct Zombie;

struct GameState {
	float t1;
	float t2;
	float t3;

	Camera camera;

	// lights
	u64 lights_count;
	u64 lights_curr;
	PointLight *lights;

	// entities
	u64 zombies_count;
	u64 zombies_curr;
	Zombie *zombies;

	GameState();
	void input();
	void update(float dt);

	static void blend(float alpha, GameState *blended, GameState *previous, GameState *current);
};

#endif
