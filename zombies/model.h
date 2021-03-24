//
// Created by vfs on 5/27/2020.
//

#ifndef ZOMBIES_MODEL_H
#define ZOMBIES_MODEL_H

struct Model {
	u64 mesh_id;
	Mesh *mesh;

	u64 shader_id;
	Shader *shader;

	u64 texture_id;
	Texture *texture;

	Sampler sampler;

	Material material;

	RasterizerState raster_state;

	Topology topology;

	VertexConstantBuffer vcb;
	MaterialBuffer mb;

	Model();
	Model(u64 mesh_id, u64 shader_id, u64 texture_id, VertexShaderConstant cb);
	void update(VertexShaderConstant cb);
	void update(VertexShaderConstant cb, Material mat);

	void set_shader(u64 shader_id);

	void render();
};

#endif
