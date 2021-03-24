//
// Created by vfs on 5/27/2020.
//

extern IDXGISwapChain *swap_chain;
extern ID3D11Device *device;
extern ID3D11DeviceContext *context;
extern ID3D11RenderTargetView *target;


extern Memory memory;
extern GameState *render_state;

extern MeshManager mesh_manager;
extern ShaderManager shader_manager;
extern TextureManager texture_manager;

Model::Model() {

}

Model::Model(u64 mesh_id, u64 shader_id, u64 texture_id, VertexShaderConstant cb) {
	// right now our models own cpu memory with vertex data and index data
	// since our game is small, maybe we can load everything to memory at init
	// and have entities own models that share this data

	// allocation in cpu, no gpu buffer
	//mesh = Mesh(path);
	this->mesh_id = mesh_id;
	mesh = mesh_manager.get_mesh(this->mesh_id);

	this->shader_id = shader_id;
	shader = shader_manager.get_shader_by_id(this->shader_id);
	if (shader == NULL) {
		LOG_WARNING("Shader couldn't be found or had compilation errors.");
		shader = shader_manager.get_shader_by_name("shaders/solid.hlsl");
		this->shader_id = shader->id;
	}

	this->texture_id = texture_id;
	texture = texture_manager.get_texture_by_id(this->texture_id);


	material = {};
	material.color = {1.0f, .0f, 1.0f, 1.0f};
	material.specular_intensity = 0.6f;
	material.specular_power = 30.0f;

	init_material_buffer(&mb, material, 1, 1);


	init_topology(&topology, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	init_vertex_constant_buffer(&vcb, cb, sizeof(cb));

	init_sampler(&sampler);

	init_rasterizer_state(&raster_state);
}

void Model::update(VertexShaderConstant cb) {
	if (shader->is_compiled == false) {
		shader = shader_manager.get_shader_by_name("shaders/solid.hlsl");
		this->shader_id = shader->id;
	}

	update_vertex_constant_buffer(&vcb, cb, sizeof(cb));
	update_material_buffer(&mb, material, sizeof(material));
}

void Model::update(VertexShaderConstant cb, Material mat) {
	update_vertex_constant_buffer(&vcb, cb, sizeof(cb));
	update_material_buffer(&mb, mat, sizeof(mat));
}

void Model::set_shader(u64 shader_id) {
	Shader *shader = shader_manager.get_shader_by_id(shader_id);

	if (shader != NULL) {
		this->shader_id = shader_id;
		this->shader = shader;
	} else {
		LOG_WARNING("Could not switch to shader");
	}
}

void Model::render() {


	bind_rasterizer_state(&raster_state);
	mesh->bind();
	bind_topology(&topology);
	bind_sampler(&sampler);
	texture->bind();

	bind_vertex_constant_buffer(&vcb);
	bind_material_buffer(&mb);

	for (u64 i = 0; i < render_state->lights_curr; ++i) {
		render_state->lights[i].bind();
	}

	shader->bind();

	context->DrawIndexed(mesh->index_buffer.count, 0, 0);
}
