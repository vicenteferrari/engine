//
// Created by vfs on 5/23/2020.
//

extern Settings settings;

extern Window_sdl window_sdl;

extern IDXGISwapChain *swap_chain;
extern ID3D11Device *device;
extern ID3D11DeviceContext *context;
extern ID3D11RenderTargetView *target;
extern ID3D11DepthStencilView *depth_view;

extern ID2D1RenderTarget *target2d;

//void init_graphics_d3d11(IDXGISwapChain *swap_chain, ID3D11Device *device, ID3D11DeviceContext *context) {
ZOMBIES_RESULT init_graphics_d3d11() {

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window_sdl.win32_handle;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
												NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &swap_chain, &device, NULL, &context);

	if (FAILED(res)) {

		char *buffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					  NULL, GetLastError(), MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR) &buffer, 0, NULL);

		MESSAGE_ERROR(buffer);
		LocalFree(buffer);

		return ZOMBIES_ERROR;
	}

	ID3D11Resource *back_buffer = NULL;
	swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), (void **) &back_buffer);
	device->CreateRenderTargetView(back_buffer, NULL, &target);
	//back_buffer->Release();

	// depth buffer

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = TRUE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ID3D11DepthStencilState *depthState;
	device->CreateDepthStencilState(&depthDesc, &depthState);

	context->OMSetDepthStencilState(depthState, 1);

	// depth texture

	ID3D11Texture2D *depthTex;
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = settings.width;
	texDesc.Height = settings.height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	device->CreateTexture2D(&texDesc, NULL, &depthTex);

	// depth view

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(depthTex, &viewDesc, &depth_view);

	context->OMSetRenderTargets(1, &target, depth_view);

	D3D11_VIEWPORT vp = {};
	vp.Width = (float) settings.width;
	vp.Height = (float) settings.height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	ID2D1Factory *factory2d = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory2d);

	// get d3d backbuffer

	float dpi_x;
	float dpi_y;
	factory2d->GetDesktopDpi(&dpi_x, &dpi_y);
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi_x,
			dpi_y);

	IDXGISurface *back_buffer3d = NULL;
	hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer3d));
	hr = factory2d->CreateDxgiSurfaceRenderTarget(back_buffer3d, &props, &target2d);


	// imgui
	ImGui_ImplDX11_Init(device, context);

	return ZOMBIES_SUCCESS;
}

void destroy_graphics() {
	ImGui_ImplDX11_Shutdown();

	if (target) target->Release();
	if (context) context->Release();
	if (device) device->Release();
	if (swap_chain) swap_chain->Release();
}

void present() {
	HRESULT hr = swap_chain->Present(settings.vsync, 0);

	if (FAILED(hr)) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			hr = device->GetDeviceRemovedReason();

		}
	}
}

void clear_buffer_d3d11(float r, float g, float b) {
	const float color[] = {r, g, b, 1.0f};
	context->ClearRenderTargetView(target, color);
	context->ClearDepthStencilView(depth_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void device_create_buffer_d3d11(const D3D11_BUFFER_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Buffer **buffer) {
	device->CreateBuffer(desc, data, buffer);
}
