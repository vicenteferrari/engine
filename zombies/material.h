//
// Created by vfs on 6/19/2020.
// 

#ifndef ZOMBIES_MATERIAL_H
#define ZOMBIES_MATERIAL_H

struct Material {
	alignas(16) dx::XMFLOAT4 color;
	alignas(4) float specular_intensity;
	alignas(4) float specular_power;
};

struct MaterialBuffer {
	Material material;
	uint32_t material_size;
	ID3D11Buffer *buffer;
	uint32_t slot;
};

void init_material_buffer(MaterialBuffer *material_buffer, Material material, uint32_t material_size, uint32_t slot);
void destroy_material_buffer(MaterialBuffer *material_buffer);
void update_material_buffer(MaterialBuffer *material_buffer, Material material, uint32_t material_size);
void bind_material_buffer(MaterialBuffer *material_buffer);

#endif
