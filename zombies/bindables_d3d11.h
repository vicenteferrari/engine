//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_BINDABLES_H
#define ZOMBIES_BINDABLES_H

// primitive data definitions

typedef const wchar_t* LPCWSTR;

typedef uint32_t Index;

struct Vertex {
	struct {
		float x;
		float y;
		float z;
	} pos;
};

struct NormVertex {
	struct {
		float x;
		float y;
		float z;
	} pos;
	struct{
		float x;
		float y;
		float z;
	} norm;
};

struct TexVertex {
	struct {
		float x;
		float y;
		float z;
	} pos;

	struct {
		float u;
		float v;
	} tex;
};

struct TexNormVertex {
	dx::XMFLOAT3 pos;

	dx::XMFLOAT3 norm;

	dx::XMFLOAT2 tex;
};



// rasterizer state

struct RasterizerState {
	ID3D11RasterizerState *state;
};

void init_rasterizer_state(RasterizerState *rasterizer_state);
void bind_rasterizer_state(RasterizerState *rasterizer_state);



// vertex buffer

struct VertexBuffer {
	uint32_t stride;
	size_t count;
	ID3D11Buffer *buffer;
};

void init_vertex_buffer(VertexBuffer *vertex_buffer, const Vertex *vertices, size_t vertex_count);
void init_texture_vertex_buffer(VertexBuffer *vertex_buffer, const TexVertex *vertices, uint32_t vertex_count);
void init_shadow_texture_vertex_buffer(VertexBuffer *vertex_buffer, const TexNormVertex *vertices, uint32_t vertex_count);
void destroy_vertex_buffer(VertexBuffer *vertex_buffer);
void bind_vertex_buffer(VertexBuffer *vertex_buffer);



// index buffer

struct IndexBuffer {
	size_t count;
	DXGI_FORMAT format;
	ID3D11Buffer *index_buffer;
};

void init_index_buffer(IndexBuffer *index_buffer, const Index *indices, size_t index_count);
void destroy_index_buffer(IndexBuffer *index_buffer);
void bind_index_buffer(IndexBuffer *index_buffer);



// vertex shader

struct VertexShaderConstant {
	dx::XMMATRIX world;
	dx::XMMATRIX world_view;
	dx::XMMATRIX world_view_proj;
};

struct VertexShader {
	ID3D11VertexShader *vertexShader;
	ID3DBlob *byteCodeBlob;
};

void init_vertex_shader(VertexShader *vertex_shader, const LPCWSTR path);
void destroy_vertex_shader(VertexShader *vertex_shader);
void bind_vertex_shader(VertexShader *vertex_shader);



// pixel shader

struct PixelShader {
	ID3D11PixelShader *pixelShader;
	ID3DBlob *byteCodeBlob;
};

void init_pixel_shader(PixelShader *pixelShader, const LPCWSTR path);
void destroy_pixel_shader(PixelShader *pixelShader);
void bind_pixel_shader(PixelShader *pixelShader);



// vertex constant buffer

struct VertexConstantBuffer {
	VertexShaderConstant constants;
	uint32_t constantsSize;
	ID3D11Buffer *constantBuffer;
};

void init_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer, VertexShaderConstant constants, uint32_t size);
void destroy_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer);
void update_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer, VertexShaderConstant constants, uint32_t size);
void bind_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer);



// color constant buffer

struct Color {
	union {
		struct {
			float r;
			float g;
			float b;
			float a;
		};

		float components[3];
	};
};

struct ColorConstantBuffer {
	Color *colors;
	uint32_t numColors;
	ID3D11Buffer *constantBuffer;
};

void init_color_constant_bcbufuffer(ColorConstantBuffer *colorConstantBuffer, Color *colors, uint32_t numColors);
void destroy_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer);
void update_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer, Color *colors, uint32_t numColors);
void bind_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer);



// point light constant buffer

struct PointLightConstant {
	alignas(16) dx::XMFLOAT3 world_pos;
	alignas(16) dx::XMFLOAT3 view_pos;
	alignas(16) dx::XMFLOAT3 ambient;
	alignas(16) dx::XMFLOAT3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

struct PointLightConstantBuffer {
	ID3D11Buffer *constantBuffer;
};

void init_pointlight_constant_buffer(PointLightConstantBuffer *buffer, PointLightConstant constants);
void destroy_pointlight_constant_buffer(PointLightConstantBuffer *pointLightConstantBuffer);
void update_pointlight_constant_buffer(PointLightConstantBuffer *plcb, PointLightConstant constants, uint32_t constantsSize);
void bind_pointlight_constant_buffer(PointLightConstantBuffer *pointLightConstantBuffer);



// input layout

struct InputLayout {
	ID3D11InputLayout *input_layout;
};

void init_input_layout(InputLayout *input_layout, const D3D11_INPUT_ELEMENT_DESC *layout, uint32_t layout_size, ID3DBlob *vertex_shader_bytecode);
void destroy_input_layout(InputLayout *input_layout);
void bind_input_layout(InputLayout *input_layout);



// topology

struct Topology {
	D3D11_PRIMITIVE_TOPOLOGY type;
};

void init_topology(Topology *topology, D3D11_PRIMITIVE_TOPOLOGY type);
void destroy_topology(Topology *topology);
void bind_topology(Topology *topology);



// sampler

struct Sampler {
	ID3D11SamplerState *sampler;
};

void init_sampler(Sampler *sampler);
void destroy_sampler(Sampler *sampler);
void bind_sampler(Sampler *sampler);

#endif
