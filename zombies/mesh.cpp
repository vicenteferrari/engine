//
// Created by vfs on 6/12/2020.
//

extern Memory memory;

extern u64 mesh_id;

Mesh::Mesh() {

}

Mesh::Mesh(u64 id, const char *path) {
	this->id = id;

	//fastObjMesh *mesh = fast_obj_read(path);

	Assimp::Importer imp;
	const aiScene *temp_model = imp.ReadFile(path, aiProcess_Triangulate);
	aiMesh *temp_mesh = temp_model->mMeshes[0];
	sprintf_s(name, "%s", temp_mesh->mName.C_Str());

	num_vertices = temp_mesh->mNumVertices;

	vertices = (TexNormVertex *) arena_alloc(&memory.temp_memory_arena, num_vertices * sizeof(TexNormVertex));

	for (u64 i = 0; i < num_vertices; ++i) {
		TexNormVertex vertex = {};
		if (temp_mesh->HasPositions())
			vertex.pos = {temp_mesh->mVertices[i].x, temp_mesh->mVertices[i].y, temp_mesh->mVertices[i].z};
		if (temp_mesh->HasNormals())
			vertex.norm = {temp_mesh->mNormals[i].x,          temp_mesh->mNormals[i].y,  temp_mesh->mNormals[i].z};
		if (temp_mesh->HasTextureCoords(0))
			vertex.tex = {temp_mesh->mTextureCoords[0][i].x, temp_mesh->mTextureCoords[0][i].y};

		vertices[i] = vertex;
	}

	num_indices = temp_mesh->mNumFaces * 3;

	indices = (Index *) arena_alloc(&memory.temp_memory_arena, num_indices * sizeof(Index));

	for (u64 i = 0; i < temp_mesh->mNumFaces; ++i) {
		aiFace face = temp_mesh->mFaces[i];
		//assert(face.mNumIndices == 3);

		for (u64 j = 0; j < face.mNumIndices; ++j) {
			indices[(i * 3) + j] = (Index) face.mIndices[j];
		}
	}

	init_shadow_texture_vertex_buffer(&vertex_buffer, vertices, num_vertices);

	init_index_buffer(&index_buffer, indices, num_indices);

	char buffer[512];
	sprintf_s(buffer, "Loaded mesh %s", name);
	LOG_INFO(buffer);
}

void Mesh::bind() {
	bind_vertex_buffer(&vertex_buffer);
	bind_index_buffer(&index_buffer);
}

ZOMBIES_RESULT init_mesh_manager(MeshManager *manager) {
	manager->meshes_count = 0;

	// first count how many .obj files are in the models folder
	WIN32_FIND_DATAA fd;
	HANDLE file = FindFirstFileA(MESH_FOLDER"/*.obj", &fd);
	++manager->meshes_count;

	while (FindNextFileA(file, &fd) != 0) {
		++manager->meshes_count;
	}

	manager->meshes_curr = 0;
	manager->meshes = (Mesh *) arena_alloc(&memory.temp_memory_arena, manager->meshes_count * sizeof(Mesh));

	// then load them
	file = FindFirstFileA(MESH_FOLDER"/*.obj", &fd);
	char path[1024];
	sprintf_s(path, MESH_FOLDER"/%s", fd.cFileName);
	new(manager->meshes) Mesh(mesh_id++, path);
	++manager->meshes_curr;

	for (u64 i = 1; FindNextFileA(file, &fd) != 0; ++i) {
		sprintf_s(path, MESH_FOLDER"/%s", fd.cFileName);
		new((Mesh *) (manager->meshes + i)) Mesh(mesh_id++, path);
		++manager->meshes_curr;
	}

	return ZOMBIES_SUCCESS;
}

Mesh *MeshManager::get_mesh(u64 id) {
	for (u64 i = 0; i < meshes_curr; ++i) {
		if (meshes[i].id == id)
			return (Mesh *) meshes + i;
	}

	return NULL;
}

void MeshManager::imgui() {
	u64 ids[MAX_MESHES];
	char names[512][MAX_MESHES];

	for (u64 i = 0; i < meshes_curr; ++i) {
		ids[i] = meshes[i].id;
		strcpy_s(names[i], 512, meshes[i].name);
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
