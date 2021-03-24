//
// Created by vfs on 6/19/2020.
//

extern ID3D11Device *device;
extern ID3D11DeviceContext *context;

void init_material_buffer(MaterialBuffer *material_buffer, Material material, uint32_t material_size, uint32_t slot) {
	material_buffer->material = material;
	material_buffer->material_size = material_size;
	material_buffer->slot = slot;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(Material);
	cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &material_buffer->material;

	HRESULT hr = device->CreateBuffer(&cbd, &csd, &material_buffer->buffer);
	if (FAILED(hr))
		assert(1 == 0);
}

void destroy_material_buffer(MaterialBuffer *material_buffer) {

}

void update_material_buffer(MaterialBuffer *material_buffer, Material material, uint32_t material_size) {
	D3D11_MAPPED_SUBRESOURCE msr;

	context->Map(material_buffer->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, &material, material_size);
	context->Unmap(material_buffer->buffer, 0);
}

void bind_material_buffer(MaterialBuffer *material_buffer) {
	context->PSSetConstantBuffers(1, 1, &material_buffer->buffer);
}
