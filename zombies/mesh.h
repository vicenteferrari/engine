//
// Created by vfs on 6/12/2020.
//

#ifndef ZOMBIES_MESH_H
#define ZOMBIES_MESH_H

#define MAX_MESHES 1024
#define MESH_FOLDER "models"

struct Mesh {
	u64 id;
	char name[512];

	u64 num_vertices;
	TexNormVertex *vertices;

	u64 num_indices;
	Index *indices;

	VertexBuffer vertex_buffer;
	IndexBuffer index_buffer;

	Mesh();
	Mesh(u64 id, const char *path);
	void bind();
};

struct MeshManager {
	u64 meshes_count;
	u64 meshes_curr;
	Mesh *meshes;

	Mesh *get_mesh(u64 id);
	void imgui();
};

ZOMBIES_RESULT init_mesh_manager(MeshManager *manager);

#endif
