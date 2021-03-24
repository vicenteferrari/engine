//
// Created by vfs on 5/27/2020.
//

#ifndef ZOMBIES_ZOMBIE_H
#define ZOMBIES_ZOMBIE_H

struct Zombie {
	GameState *state;

	NZombie n_zombie;

	Model model;

	// for imgui
	char meshes[512][MAX_MESHES];
	char shaders[512][MAX_SHADERS];
	char textures[512][MAX_SHADERS];

	Zombie();
	Zombie(GameState *state);
	void update(float dt);
	void render();
	VertexShaderConstant get_transform();
	void imgui();

	static void blend(float alpha, Zombie *blended, Zombie *previous, Zombie *current);
};

#endif
