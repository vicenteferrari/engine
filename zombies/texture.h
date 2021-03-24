//
// Created by vfs on 7/5/2020.
//

#ifndef ZOMBIES_TEXTURE_H
#define ZOMBIES_TEXTURE_H

#define MAX_TEXTURES 1024
#define MAX_TEXTURE_SIZE MEGABYTES(10)
#define TEXTURE_FOLDER "textures"

struct Texture {
    u64 id;
    char name[512];

    u8 *data;

    ID3D11Texture2D *device_texture;
	ID3D11ShaderResourceView *texture_view;

    Texture();
    Texture(u64 id, const char *path);
    void bind();
};

struct TextureManager {
    u64 textures_count;
    u64 textures_curr;
    Texture *textures;

    Texture *get_texture_by_id(u64 id);
    void imgui();
};

ZOMBIES_RESULT init_texture_manager(TextureManager *manager);

#endif
