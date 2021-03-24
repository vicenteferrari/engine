//
// Created by vfs on 29-Jun-20.
//

#ifndef ZOMBIES_N_POINT_LIGHT_H
#define ZOMBIES_N_POINT_LIGHT_H

struct NPointLight {
	Vec3 position{};
	Vec3 velocity{};

	Vec3 orientation{};
	Vec3 angular{};

	ZOMBIESCOMMON_API
	NPointLight();

	ZOMBIESCOMMON_API
	void update(float dt);

	ZOMBIESCOMMON_API
	static void blend(float alpha, NPointLight *blended, NPointLight *previous, NPointLight *current);
};

#endif //ZOMBIES_N_POINT_LIGHT_H
