//
// Created by vfs on 6/14/2020.
//

#ifndef ZOMBIES_SHADER_H
#define ZOMBIES_SHADER_H

#define MAX_SHADERS 1024
#define MAX_SHADER_LENGTH 10 * 1024
#define SHADER_FOLDER "shaders"

// maybe at some point we'll have separate structs for vertex and pixel shaders
// right now they always come in pairs.

struct Shader {
	uint64_t id;
	char name[512];
	bool is_compiled;

	char path[512];
	HANDLE file;
	FILETIME creation_time;
	FILETIME last_access_time;
	FILETIME last_write_time;

	char data[MAX_SHADER_LENGTH];
	uint32_t data_size;

	ID3D11VertexShader *vertex_shader;
	ID3DBlob *vertex_blob;
	ID3DBlob *vertex_error_blob;

	ID3D11PixelShader *pixel_shader;
	ID3DBlob *pixel_blob;
	ID3DBlob *pixel_error_blob;

	InputLayout input_layout;

	Shader();
	Shader(uint64_t id, const char *path);
	ZOMBIES_RESULT compile();
	void release();
	ZOMBIES_RESULT hot_reload();
	void bind();
};

struct ShaderManager {
	size_t shaders_count;
	size_t shaders_curr;
	Shader *shaders;

	Shader *get_shader_by_id(uint64_t id);
	Shader *get_shader_by_name(const char *name);
	void hot_reloads();
	void imgui();
};

ZOMBIES_RESULT init_shader_manager(ShaderManager *manager);

#endif
