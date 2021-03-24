//
// Created by vfs on 6/14/2020.
//

extern ID3D11Device *device;
extern ID3D11DeviceContext *context;

extern Memory memory;
extern u64 shader_id;

Shader::Shader() {

}

Shader::Shader(uint64_t id, const char *path) {
	this->id = id;
	is_compiled = false;

	sprintf_s(name, "%s", path);
	sprintf_s(this->path, "%s", path);

	vertex_shader = NULL;
	vertex_blob = NULL;
	vertex_error_blob = NULL;

	pixel_shader = NULL;
	pixel_blob = NULL;
	pixel_error_blob = NULL;

	if (compile() == ZOMBIES_SUCCESS) {
		char buffer[512];
		sprintf_s(buffer, "Loaded shader %s", name);
		LOG_INFO(buffer);

		is_compiled = true;
	} else {
		char buffer[512];
		sprintf_s(buffer, "Could not load shader %s", name);
		LOG_WARNING(buffer);
	}

	file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(file, &creation_time, &last_access_time, &last_write_time);
//	CloseHandle(file);
//	file = NULL;
}

void Shader::release() {
	if (vertex_blob != NULL) {
		vertex_blob->Release();
		vertex_blob = NULL;
	}

	if (vertex_shader != NULL) {
		vertex_shader->Release();
		vertex_shader = NULL;
}

	if (vertex_error_blob != NULL) {
		vertex_error_blob->Release();
		vertex_error_blob = NULL;
	}

	if (pixel_blob != NULL) {
		pixel_blob->Release();
		pixel_blob = NULL;
	}

	if (pixel_shader != NULL) {
		pixel_shader->Release();
		pixel_shader = NULL;
	}

	if (pixel_error_blob != NULL) {
		pixel_error_blob->Release();
		pixel_error_blob = NULL;
	}
}

ZOMBIES_RESULT Shader::compile() {
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(ZOMBIES_DEBUG)
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	wchar_t Lpath[256];
	mbstowcs(Lpath, path, 256);

	release();

	D3D_SHADER_MACRO vertex_macro[] = {"VERTEX", "1", NULL, NULL};
	HRESULT hr = D3DCompileFromFile(Lpath, vertex_macro, NULL, "main", "vs_5_0", flags, 0, &vertex_blob, &vertex_error_blob);

	if (SUCCEEDED(hr)) {
		hr = device->CreateVertexShader(vertex_blob->GetBufferPointer(), vertex_blob->GetBufferSize(), NULL, &vertex_shader);
	} else if (hr == ERROR_SHARING_VIOLATION) {
		compile();
	} else if (vertex_error_blob) {
		char buffer[4096];
		sprintf_s(buffer, "%s", (char *) vertex_error_blob->GetBufferPointer());
		LOG_WARNING(buffer);

		return ZOMBIES_ERROR;
	}

	D3D_SHADER_MACRO pixel_macro[] = {"PIXEL", "1", NULL, NULL};
	hr = D3DCompileFromFile(Lpath, pixel_macro, NULL, "main", "ps_5_0", flags, 0, &pixel_blob, &pixel_error_blob);

	if (SUCCEEDED(hr)) {
		hr = device->CreatePixelShader(pixel_blob->GetBufferPointer(), pixel_blob->GetBufferSize(), NULL, &pixel_shader);
	} else {
		char buffer[2048];
		sprintf_s(buffer, "%s", (char *) pixel_error_blob->GetBufferPointer());
		LOG_WARNING(buffer);

		return ZOMBIES_ERROR;
	}

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	init_input_layout(&input_layout, ied, ARRAYCOUNT(ied), vertex_blob);

	return ZOMBIES_SUCCESS;
}

ZOMBIES_RESULT Shader::hot_reload() {
	FILETIME new_creation_time;
	FILETIME new_last_access_time;
	FILETIME new_last_write_time;

	GetFileTime(file, &new_creation_time, &new_last_access_time, &new_last_write_time);

	bool dirty = CompareFileTime(&last_write_time, &new_last_write_time) == -1;

	if (dirty) {
		creation_time = new_creation_time;
		last_access_time = new_last_access_time;
		last_write_time = new_last_write_time;

		char buffer[256];
		sprintf_s(buffer, "Reloading shader %s", name);
		LOG_INFO(buffer);
		if (compile() == ZOMBIES_SUCCESS) {
			is_compiled = true;
			return ZOMBIES_SUCCESS;
		} else {
			is_compiled = false;
			return ZOMBIES_ERROR;
		}
	}

	return ZOMBIES_SUCCESS;
}

void Shader::bind() {
	bind_input_layout(&input_layout);

	context->VSSetShader(vertex_shader, NULL, 0);
	context->PSSetShader(pixel_shader, NULL, 0);
}

void get_filenames_in_dir(const char *directory, uint32_t *filename_count, char filenames[128][256]) {
	// turn directory into glob
	char glob[256];
	sprintf_s(glob, "%s/*.hlsl", directory);

	// if filenames is null, only return amount

	// first count how many .hlsl files are in the models folder

	uint32_t count = 0;

	WIN32_FIND_DATAA fd;
	HANDLE file = FindFirstFileA(glob, &fd);

	if (file != INVALID_HANDLE_VALUE) {
		if (filenames != NULL) {
			sprintf_s(filenames[0], 256, "%s%s", directory, fd.cFileName);
		}
		++count;

		for (int i = 1; i < 128; ++i) {
			if (FindNextFileA(file, &fd) != 0) {
				sprintf_s(filenames[i], 256, "%s%s", directory, fd.cFileName);

				if (filenames != NULL)
				++count;
			}
		}
	}

	*filename_count = count;
}

ZOMBIES_RESULT init_shader_manager(ShaderManager *manager) {
	manager->shaders_count = 0;

	uint32_t filename_count = 0;
	char filenames[128][256] = {};
	get_filenames_in_dir("shaders/", &filename_count, filenames);

	manager->shaders_count = filename_count;

	manager->shaders_curr = 0;
	manager->shaders = (Shader *) arena_alloc(&memory.temp_memory_arena, manager->shaders_count * sizeof(Shader));

	// then load them

	for (int i = 0; i < filename_count; ++i) {
		new ((Shader *)(manager->shaders + i)) Shader(shader_id++, filenames[i]);
		++manager->shaders_curr;
	}

	return ZOMBIES_SUCCESS;
}

Shader *ShaderManager::get_shader_by_id(uint64_t id) {
	for (int i = 0; i < shaders_curr; ++i) {
		if (shaders[i].id == id) {
			if (shaders[i].is_compiled) {
				return (Shader *) shaders + i;
			} else {
				return NULL;
			}
		}
	}

	return NULL;
}

Shader *ShaderManager::get_shader_by_name(const char *name) {
	for (int i = 0; i < shaders_curr; ++i) {
		if (strcmp(name, shaders[i].name) == 0) {
			if (shaders[i].is_compiled) {
				return (Shader *) shaders + i;
			} else {
				return NULL;
			}
		}
	}

	return NULL;
}

void ShaderManager::hot_reloads() {
	for (int i = 0; i < shaders_curr; ++i) {
		shaders[i].hot_reload();
	}
}

void ShaderManager::imgui() {

}
