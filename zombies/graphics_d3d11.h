//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_GRAPHICS_H
#define ZOMBIES_GRAPHICS_H

enum vsync_d3d11 {
	NO_VSYNC_D3D11,
	VSYNC_D3D11,
	VSYNC2_D3D11,
	VSYNC3_D3D11,
	VSYNC4_D3D11,
};

//void init_graphics_d3d11(IDXGISwapChain *swap_chain, ID3D11Device *device, ID3D11DeviceContext *context);
ZOMBIES_RESULT init_graphics_d3d11();
void destroy_graphics();
void present();
void clear_buffer_d3d11(float r, float g, float b);
void device_create_buffer_d3d11(const D3D11_BUFFER_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Buffer **buffer);

#endif
