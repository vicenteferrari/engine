//
// Created by vfs on 2020-09-19.
//

#ifndef ZOMBIES_MAKE_GRAPHICS_OPENGL_H
#define ZOMBIES_MAKE_GRAPHICS_OPENGL_H

enum vsync_opengl {
	NO_VSYNC_OPENGL,
	VSYNC_OPENGL,
	VSYNC2_OPENGL,
	VSYNC3_OPENGL,
	VSYNC4_OPENGL,
};

//void init_graphics_d3d11(IDXGISwapChain *swap_chain, ID3D11Device *device, ID3D11DeviceContext *context);
ZOMBIES_RESULT init_graphics_opengl();
void destroy_graphics();
void present();
void clear_buffer_opengl(float r, float g, float b);
void device_create_buffer_opengl(const D3D11_BUFFER_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Buffer **buffer);

#endif //ZOMBIES_MAKE_GRAPHICS_OPENGL_H
