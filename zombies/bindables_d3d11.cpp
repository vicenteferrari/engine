//
// Created by vfs on 5/23/2020.
//

extern IDXGISwapChain *swap_chain;
extern ID3D11Device *device;
extern ID3D11DeviceContext *context;
extern ID3D11RenderTargetView *target;

// rasterizer state

void init_rasterizer_state(RasterizerState *rasterizer_state) {
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FrontCounterClockwise = false;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;
	device->CreateRasterizerState(&rasterizerState, &rasterizer_state->state);
}

void bind_rasterizer_state(RasterizerState *rasterizer_state) {
	context->RSSetState(rasterizer_state->state);
}



// vertex buffer

void init_vertex_buffer(VertexBuffer *vertex_buffer, const Vertex *vertices, size_t vertex_count) {

	vertex_buffer->stride = sizeof(Vertex);
	vertex_buffer->count = vertex_count;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = UINT(vertex_count * vertex_buffer->stride);
	desc.StructureByteStride = vertex_buffer->stride;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	device_create_buffer_d3d11(&desc, &data, &vertex_buffer->buffer);
}

void init_texture_vertex_buffer(VertexBuffer *vertex_buffer, const TexVertex *vertices, uint32_t vertex_count) {

	vertex_buffer->stride = sizeof(TexVertex);

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.ByteWidth = UINT(vertex_count * vertex_buffer->stride);
	vertexBufferDesc.StructureByteStride = vertex_buffer->stride;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = vertices;

	device->CreateBuffer(&vertexBufferDesc, &bufferData, &vertex_buffer->buffer);
}

void init_shadow_texture_vertex_buffer(VertexBuffer *vertex_buffer, const TexNormVertex *vertices, uint32_t vertex_count) {

	vertex_buffer->stride = sizeof(TexNormVertex);

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.ByteWidth = UINT(vertex_count * vertex_buffer->stride);
	vertexBufferDesc.StructureByteStride = vertex_buffer->stride;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = vertices;

	device->CreateBuffer(&vertexBufferDesc, &bufferData, &vertex_buffer->buffer);
}

void destroy_vertex_buffer(VertexBuffer *vertex_buffer) {

}

void bind_vertex_buffer(VertexBuffer *vertex_buffer) {
	const uint32_t offset = 0;
	context->IASetVertexBuffers(0, 1, &vertex_buffer->buffer, &vertex_buffer->stride, &offset);
}



// index buffer

void init_index_buffer(IndexBuffer *index_buffer, const Index *indices, size_t index_count) {

	index_buffer->count = index_count;

	if (sizeof(Index) == 2) {
		index_buffer->format = DXGI_FORMAT_R16_UINT;
	} else if (sizeof(Index) == 4) {
		index_buffer->format = DXGI_FORMAT_R32_UINT;
	}

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = index_count * sizeof(Index);
	indexBufferDesc.StructureByteStride = sizeof(Index);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	device->CreateBuffer(&indexBufferDesc, &isd, &index_buffer->index_buffer);
}

void destroy_index_buffer(IndexBuffer *index_buffer) {

}

void bind_index_buffer(IndexBuffer *index_buffer) {
	context->IASetIndexBuffer(index_buffer->index_buffer, index_buffer->format, 0);
}



// vertex shader

void init_vertex_shader(VertexShader *vertex_shader, const LPCWSTR path) {

	HRESULT hr = D3DCompileFromFile(path, NULL, NULL, "main", "vs_5_0", 0, 0, &vertex_shader->byteCodeBlob, NULL);

	if (hr == S_OK) {
		device->CreateVertexShader(vertex_shader->byteCodeBlob->GetBufferPointer(),
								   vertex_shader->byteCodeBlob->GetBufferSize(), NULL,
								   &vertex_shader->vertexShader);
	}
}

void destroy_vertex_shader(VertexShader *vertex_shader) {

}

void bind_vertex_shader(VertexShader *vertex_shader) {
	context->VSSetShader(vertex_shader->vertexShader, NULL, 0);
}



// pixel shader

void init_pixel_shader(PixelShader *pixelShader, const LPCWSTR path) {
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(ZOMBIES_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif
	// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.
	LPCSTR profile = ( device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "ps_5_0" : "ps_4_0";
	const D3D_SHADER_MACRO defines[] =
			{
					"EXAMPLE_DEFINE", "1",
					NULL, NULL
			};

	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile((LPCWSTR)path, defines, NULL,
									 "main", profile,
									 flags, 0, &pixelShader->byteCodeBlob, &errorBlob);

	if (FAILED(hr)) {
		char buffer[2048];
		sprintf_s(buffer, "%s", (char *) errorBlob->GetBufferPointer());
		printf(buffer);
		MESSAGE_ERROR(buffer);
	} else {
		device->CreatePixelShader(pixelShader->byteCodeBlob->GetBufferPointer(), pixelShader->byteCodeBlob->GetBufferSize(),
								  NULL, &pixelShader->pixelShader);
	}
}

void destroy_pixel_shader(PixelShader *pixelShader) {

}

void bind_pixel_shader(PixelShader *pixelShader) {
	context->PSSetShader(pixelShader->pixelShader, NULL, 0);
}



// vertex constant buffer

void init_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer, VertexShaderConstant constants, uint32_t size) {

	vertex_constant_buffer->constants = constants;
	vertex_constant_buffer->constantsSize = size;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = vertex_constant_buffer->constantsSize;
	cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &vertex_constant_buffer->constants;
	device->CreateBuffer(&cbd, &csd, &vertex_constant_buffer->constantBuffer);
}

void destroy_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer) {

}

void update_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer, VertexShaderConstant constants, uint32_t size) {

	D3D11_MAPPED_SUBRESOURCE msr;

	context->Map(vertex_constant_buffer->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, &constants, size);
	context->Unmap(vertex_constant_buffer->constantBuffer, 0);
}

void bind_vertex_constant_buffer(VertexConstantBuffer *vertex_constant_buffer) {
	context->VSSetConstantBuffers(0, 1, &vertex_constant_buffer->constantBuffer);
}



// pixel constant buffer

void init_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer, Color *colors, uint32_t numColors) {

	colorConstantBuffer->colors = colors;
	colorConstantBuffer->numColors = numColors;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = colorConstantBuffer->numColors * sizeof(Color);
	cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = colorConstantBuffer->colors;
	device->CreateBuffer(&cbd, &csd, &colorConstantBuffer->constantBuffer);
}

void destroy_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer) {

}

void update_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer, Color *colors, uint32_t numColors) {

	D3D11_MAPPED_SUBRESOURCE msr;

	context->Map(colorConstantBuffer->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, colors, colorConstantBuffer->numColors * sizeof(Color));
	context->Unmap(colorConstantBuffer->constantBuffer, 0);
}

void bind_color_constant_buffer(ColorConstantBuffer *colorConstantBuffer) {
	context->PSSetConstantBuffers(0, 1, &colorConstantBuffer->constantBuffer);
}



// light pos const buffer

void init_pointlight_constant_buffer(PointLightConstantBuffer *buffer, PointLightConstant constants) {

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(constants);
	cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &constants;
	device->CreateBuffer(&cbd, &csd, &buffer->constantBuffer);
}

void destroy_pointlight_constant_buffer(PointLightConstantBuffer *pointLightConstantBuffer) {

}

void update_pointlight_constant_buffer(PointLightConstantBuffer *plcb, PointLightConstant constants, uint32_t constantsSize) {
	D3D11_MAPPED_SUBRESOURCE msr;

	context->Map(plcb->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, &constants, constantsSize);
	context->Unmap(plcb->constantBuffer, 0);
}

void bind_pointlight_constant_buffer(PointLightConstantBuffer *pointLightConstantBuffer) {
	context->PSSetConstantBuffers(0, 1, &pointLightConstantBuffer->constantBuffer);
}



// input layout

void init_input_layout(InputLayout *input_layout,
					   const D3D11_INPUT_ELEMENT_DESC *layout, uint32_t layout_size,
					   ID3DBlob *vertex_shader_bytecode) {

	device->CreateInputLayout(layout, layout_size, vertex_shader_bytecode->GetBufferPointer(),
							  vertex_shader_bytecode->GetBufferSize(), &input_layout->input_layout);
}

void destroy_input_layout(InputLayout *input_layout) {

}

void bind_input_layout(InputLayout *input_layout) {
	context->IASetInputLayout(input_layout->input_layout);
}



// topology

void init_topology(Topology *topology, D3D11_PRIMITIVE_TOPOLOGY type) {
	topology->type = type;
}

void destroy_topology(Topology *topology) {

}

void bind_topology(Topology *topology) {
	context->IASetPrimitiveTopology(topology->type);
}



// sampler

void init_sampler(Sampler *sampler) {
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	device->CreateSamplerState(&samplerDesc, &sampler->sampler);
}

void destroy_sampler(Sampler *sampler) {

}

void bind_sampler(Sampler *sampler) {
	context->PSSetSamplers(0, 1, &sampler->sampler);
}
