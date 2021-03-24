//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_POINT_LIGHT_H
#define ZOMBIES_POINT_LIGHT_H

struct GameState;

struct PointLight {
	NPointLight n_point_light;

	dx::XMFLOAT3 ambient;
	dx::XMFLOAT3 diffuse_color;
	float diffuse_intensity;
	float atten_const;
	float atten_lin;
	float atten_quad;

	PointLightConstantBuffer buffer;

	GameState *state;

	Model model;

	PointLight(GameState *state);
	void update();
	void render();
	void imgui();
	void bind();
	void reset();
	PointLightConstant get_constant();
	VertexShaderConstant get_transform();

	static void blend(float alpha, PointLight *blended, PointLight *previous, PointLight *current);
};

void init_pointlight(PointLight *pointlight, GameState *state, float x, float y, float z);

#endif
