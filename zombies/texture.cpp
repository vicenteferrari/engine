//
// Created by vfs on 7/5/2020.
//

extern Memory memory;
extern ID3D11DeviceContext *context;

extern u64 texture_id;

Texture::Texture() {

}

Texture::Texture(u64 id, const char *path) {
    this->id = id;
    strcpy_s(this->name, 512, path);

    int x, y, n;
	this->data = stbi_load(path, &x, &y, &n, 4);

    D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Height = y;
	texture_desc.Width = x;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = this->data;
	sd.SysMemPitch = x * 4 * sizeof(uint8_t);

	device->CreateTexture2D(&texture_desc, &sd, &this->device_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
	view_desc.Format = texture_desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = 0;
	view_desc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(this->device_texture, &view_desc, &this->texture_view);

	char buffer[512];
	sprintf_s(buffer, "Loaded texture %s", name);
	LOG_INFO(buffer);
}

void Texture::bind() {
	context->PSSetShaderResources(0, 1, &texture_view);
}

ZOMBIES_RESULT init_texture_manager(TextureManager *manager) {
    manager->textures_count = 0;

    // first count how many .obj files are in the models folder
	WIN32_FIND_DATAA fd;
	HANDLE file = FindFirstFileA(TEXTURE_FOLDER"/*.png", &fd);
	++manager->textures_count;

	while (FindNextFileA(file, &fd) != 0) {
		++manager->textures_count;
	}

	manager->textures_curr = 0;
	manager->textures = (Texture *) arena_alloc(&memory.temp_memory_arena, manager->textures_count * sizeof(Texture));

	// then load them
	file = FindFirstFileA(TEXTURE_FOLDER"/*.png", &fd);
	char path[1024];
	sprintf_s(path, TEXTURE_FOLDER"/%s", fd.cFileName);
	new(manager->textures) Texture(texture_id++, path);
	++manager->textures_curr;

	for (u64 i = 1; FindNextFileA(file, &fd) != 0; ++i) {
		sprintf_s(path, TEXTURE_FOLDER"/%s", fd.cFileName);
		new((Mesh *) (manager->textures + i)) Texture(texture_id++, path);
		++manager->textures_curr;
	}

	return ZOMBIES_SUCCESS;
}

Texture *TextureManager::get_texture_by_id(u64 id) {
    for (u64 i = 0; i < textures_curr; ++i) {
		if (textures[i].id == id)
			return (Texture *) textures + i;
	}

	return NULL;
}

void TextureManager::imgui() {
	u64 ids[MAX_TEXTURES];
	char names[512][512];

	for (u64 i = 0; i < textures_curr; ++i) {
		ids[i] = textures[i].id;
		strcpy_s(names[i], 512, textures[i].name);
	}

//	if (ImGui::Begin("Mesh Manager")) {
//		ImGui::Text("Position");
//		ImGui::SliderFloat3("Position", &position.x, -8.0f, 8.0f, "%.2f");
//		ImGui::SliderFloat3("Orientation", &orientation.x, -8.0f, 8.0f, "%.2f");
//
//		ImGui::Text("Material");
//		ImGui::SliderFloat("Specular Intensity", &model.material.specularIntensity, 0.0f, 10.0f, "%.2f", 2);
//		ImGui::SliderFloat("Specular Power", &model.material.specularPower, 0.0f, 50.0f, "%.2f", 2);
//		ImGui::ColorEdit3("Color", &model.material.color.x);
//
//		if (ImGui::Button("Reset")) {
//			//reset();
//		}
//	}
//	ImGui::End();
}
