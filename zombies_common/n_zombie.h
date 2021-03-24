//
// Created by vfs on 29-Jun-20.
//

#ifndef ZOMBIES_N_ZOMBIE_H
#define ZOMBIES_N_ZOMBIE_H

struct NZombie{
	Vec3 position;
	Vec3 velocity;

	Vec3 orientation;
	Vec3 angular;

	ZOMBIESCOMMON_API
	NZombie();

	ZOMBIESCOMMON_API
	void update(float dt);

	ZOMBIESCOMMON_API
	static void blend(float alpha, NZombie *blended, NZombie *previous, NZombie *current);
};

#endif //ZOMBIES_N_ZOMBIE_H
