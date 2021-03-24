//
// Created by vfs on 5/23/2020.
//

extern Window_sdl window_sdl;

extern VirtualController virtual_controller;

extern Memory memory;
extern Client client;

extern GameState *previous_state;
extern GameState *current_state;
extern GameState *render_state;

extern MeshManager mesh_manager;
extern ShaderManager shader_manager;
extern TextureManager texture_manager;

//extern LPDIRECTSOUNDBUFFER second_buffer;

extern IDXGISwapChain *swap_chain;
extern ID3D11Device *device;
extern ID3D11DeviceContext *context;
extern ID3D11RenderTargetView *target;

extern ID2D1RenderTarget *target2d;

extern double t;
extern double gt;
extern float dt;
extern float k;
extern u64 frame;
extern u64 qpc_freq;

extern Settings settings;

extern RingBuffer ring_time;
extern RingBuffer ring_acc;
extern RingBuffer ring_ft;
extern RingBuffer ring_rt;
extern RingBuffer ring_ut;

extern u64 mesh_id;
extern u64 shader_id;
extern u64 material_id;
extern u64 texture_id;
extern u64 model_id;
extern u64 entity_id;
extern u64 sound_id;
extern u64 music_id;

void render_text();
HRESULT LoadBitmapFromFile(PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap);

ZOMBIES_RESULT init(int argc, char *argv[]) {
	// init game globals

	SDL_Init(SDL_INIT_EVERYTHING);

	if (init_settings(&settings) == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished initializing settings");

	if (process_arguments(&settings, argc, argv) == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished processing arguments");

	if (settings.video_driver == OPENGL) {
		s32 amount_drivers = SDL_GetNumRenderDrivers();
		SDL_RendererInfo infos[amount_drivers];
		for (int i = 0; i < amount_drivers; ++i) {
			SDL_GetRenderDriverInfo(i, &infos[i]);
		}
	}

//	if (init_window_win32(window_width, window_height) == ZOMBIES_ERROR)
	if (init_window_sdl() == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished creating window");

	if (init_graphics_d3d11() == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished creating window");

	render_logo();

	//client = Client(Address(127, 0, 0, 1, DEFAULT_PORT));
	//printf("init networking done\n");

	/* if (init_sound(window.window_handle, 48000, 48000 * 2 * sizeof(s16)) == ZOMBIES_ERROR) */
	/* 	return ZOMBIES_ERROR; */
	//fill_sound_buffer(&sound_output, 0, sound_output.bufferSize);
	//second_buffer->Play(0, 0, DSBPLAY_LOOPING);
	LOG_INFO("Finished initializing sound");

	if (init_memory(&memory) == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished initializing memory arena");

	if (init_client() == ZOMBIES_ERROR)
		return ZOMBIES_ERROR;
	LOG_INFO("Finished initializing the network client");

	if (init_mesh_manager(&mesh_manager) == ZOMBIES_ERROR) {
		return ZOMBIES_ERROR;
	}
	LOG_INFO("Finished loading all meshes");

	if (init_shader_manager(&shader_manager) == ZOMBIES_ERROR) {
		return ZOMBIES_ERROR;
	}
	LOG_INFO("Finished loading all shaders");

	if (init_texture_manager(&texture_manager) == ZOMBIES_ERROR) {
		return ZOMBIES_ERROR;
	}
	LOG_INFO("Finished loading all textures");

	assert(memory.state_memory_size > sizeof(GameState) * 3);

	//	previous_state = (GameState *) memory.state_memory;
	//	current_state = (GameState *) (((GameState *) memory.state_memory) + 1);
	//	render_state = (GameState *) (((GameState *) memory.state_memory) + 2);

	previous_state = new((GameState *) memory.state_memory) GameState();
	current_state = new((GameState *) (((GameState *) memory.state_memory) + 1)) GameState();
	render_state = new((GameState *) (((GameState *) memory.state_memory) + 2)) GameState();

	//	if (init_state(previous_state, &memory) == ZOMBIES_ERROR)
	//		return ZOMBIES_ERROR;
	//
	//	if (init_state(current_state, &memory) == ZOMBIES_ERROR)
	//		return ZOMBIES_ERROR;
	//
	//	if(init_state(render_state, &memory) == ZOMBIES_ERROR)
	//		return ZOMBIES_ERROR;

	LOG_INFO("Finished initializing game states");

	return ZOMBIES_SUCCESS;
}

void input() {
//	GameInput input[2];
//	GameInput *newInput = &input[0];
//	GameInput *oldInput = &input[1];
//
//	update_input(oldInput, newInput);
	poll_virtual_controller();

	current_state->input();
}

void update(float dt) {

	client.read();

	current_state->update(dt);

	//update_sound();
}

void render() {
	clear_buffer_d3d11(0.0f, 0.0f, 0.0f);
	//	clear_buffer_d3d11(render_state->t1, render_state->t2, render_state->t3);

	for (u32 i = 0; i < render_state->lights_curr; ++i) {
		render_state->lights[i].render();
	}

	for (u32 i = 0; i < render_state->zombies_curr; ++i) {
		render_state->zombies[i].render();
	}

#ifdef ZOMBIES_DEBUG
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_sdl.window);
	ImGui::NewFrame();

	if (ImGui::Begin("Game")) {
		ImGui::SliderFloat("Time constant k", &k, 0.001f, 2.0f);
		if (ImGui::Button("Reset")) {
			k = 1.0f;
		}
		ImGui::Text("Running real time %.3f s", t);
		ImGui::Text("Running game time %.3f s", gt);
		ImGui::Text("Application average %.3f ms/frame, %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Mouse (x, y): (%d, %d)", virtual_controller.mouse.x, virtual_controller.mouse.y);
		ImGui::Text("Raw Mouse (x, y): (%d, %d)", virtual_controller.raw_mouse.dx, virtual_controller.raw_mouse.dy);
		if (virtual_controller.keyboard.W) {
			ImGui::Text("W: pressed");
		} else {
			ImGui::Text("W: not");
		}
	}
	ImGui::End();

	client.imgui();

	// static bool show_imgui_demo = true;
	// if (show_imgui_demo) {
	// 	ImGui::ShowDemoWindow(&show_imgui_demo);
	// }

	//	static bool show_implot_demo = true;
	//	if (show_implot_demo) {
	//		ImPlot::ShowDemoWindow(&show_implot_demo);
	//	}

	ImGui::SetNextWindowSize(ImVec2(420, 490), ImGuiCond_Once);
	if (ImGui::Begin("Debug")) {

		if (ImGui::Button("BREAK!")) {
			DebugBreak();
		}

		const char *items[] = {"No VSync", "VSync 1", "VSync 2", "VSync 3", "VSync 4"};
		ImGui::Combo("VSync", &settings.vsync, items, ARRAYCOUNT(items));

		ImPlot::SetNextPlotLimitsY(0.0f, 0.02f, ImGuiCond_Always);
		ImPlot::SetNextPlotLimitsX(t - 3, t, ImGuiCond_Always);
		if (ImPlot::BeginPlot("Timings", "running real time (s)", "time (s)", ImVec2(400, 200))) {

			float flat_time[SAMPLES_COUNT] = {};

			float flat_ft[SAMPLES_COUNT] = {};
			float flat_rt[SAMPLES_COUNT] = {};
			float flat_ut[SAMPLES_COUNT] = {};

			rb_read(&ring_time, flat_time, ARRAYCOUNT(flat_time));

			rb_read(&ring_ft, flat_ft, ARRAYCOUNT(flat_ft));
			rb_read(&ring_rt, flat_rt, ARRAYCOUNT(flat_rt));
			rb_read(&ring_ut, flat_ut, ARRAYCOUNT(flat_ut));

			ImPlot::PushStyleColor(0, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImPlot::PlotLine("frame time", flat_time, flat_ft, SAMPLES_COUNT);
			ImPlot::PopStyleColor();

			ImPlot::PushStyleColor(0, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			ImPlot::PlotLine("render time", flat_time, flat_rt, SAMPLES_COUNT);
			ImPlot::PopStyleColor();

			ImPlot::PushStyleColor(0, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
			ImPlot::PlotLine("update time", flat_time, flat_ut, SAMPLES_COUNT);
			ImPlot::PopStyleColor();

			ImPlot::EndPlot();
		}

		ImPlot::SetNextPlotLimitsY(0.0f, 0.02f, ImGuiCond_Always);
		ImPlot::SetNextPlotLimitsX(t - 3, t, ImGuiCond_Always);
		if (ImPlot::BeginPlot("Accumulator remaining after all updates", "running real time (s)", "time (s)", ImVec2(400, 200))) {

			float flat_time[SAMPLES_COUNT] = {};

			float flat_acc[SAMPLES_COUNT] = {};

			rb_read(&ring_time, flat_time, ARRAYCOUNT(flat_time));

			rb_read(&ring_acc, flat_acc, ARRAYCOUNT(flat_acc));

			ImPlot::PlotLine("accumulator", flat_time, flat_acc, SAMPLES_COUNT);

			ImPlot::EndPlot();
		}
	}
	ImGui::End();
	//renderConsole(&gameState->console);

	current_state->camera.imgui();

	for (u64 i = 0; i < current_state->lights_curr; ++i) {
		current_state->lights[i].imgui();
	}

	for (u64 i = 0; i < current_state->lights_curr; ++i) {
		current_state->zombies[i].imgui();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

//	render_text();

	present();

//	raw_mouse.dx = 0;
//	raw_mouse.dy = 0;
}

void render_gradient() {
	D2D1_SIZE_F target_size = target2d->GetSize();
	ID2D1LinearGradientBrush *m_pLinearGradientBrush;
	ID2D1GradientStopCollection *pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::ForestGreen, 1);
	gradientStops[1].position = 1.0f;

	// create the ID2D1GradientStopCollection from a previously
	// declared array of D2D1_GRADIENT_STOP structs.
	HRESULT hr = target2d->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_WRAP,
			&pGradientStops
			);

	if (SUCCEEDED(hr)) {
		hr = target2d->CreateLinearGradientBrush(
				D2D1::LinearGradientBrushProperties(
					D2D1::Point2F(0.0f, 0.0f),
					D2D1::Point2F(0.01f, 0.01f)),
				pGradientStops,
				&m_pLinearGradientBrush
				);
	}

	m_pLinearGradientBrush->SetTransform(D2D1::Matrix3x2F::Scale(target_size));
	D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, target_size.width / 2, target_size.height);

	target2d->BeginDraw();

	target2d->FillRectangle(&rect, m_pLinearGradientBrush);

	target2d->EndDraw();
}

//void render_text() {
//	IDWriteFactory* pDWriteFactory_;
//	IDWriteTextFormat* pTextFormat_;
//
//	ID2D1Factory* pD2DFactory_;
//	ID2D1SolidColorBrush* pBlackBrush_;
//
//	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_);
//
//	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory_));
//
//	const wchar_t* lpText = L"Hello, عندما يريد العالم أن Sailor!\nこんいちは";
//
//
//	UINT32 cTextLength_;
//	//MessageBoxW(NULL, buffer55, buffer55, MB_OK | MB_ICONERROR);
//	cTextLength_ = (UINT32) wcslen(lpText);
//
//	pDWriteFactory_->CreateTextFormat(
//			L"Calibri",                // Font family name.
//			NULL,                       // Font collection (NULL sets it to use the system font collection).
//			DWRITE_FONT_WEIGHT_REGULAR,
//			DWRITE_FONT_STYLE_NORMAL,
//			DWRITE_FONT_STRETCH_NORMAL,
//			26.0f,
//			L"en-us",
//			&pTextFormat_
//			);
//
//	pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
//	pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
//
//	RECT rc;
//	GetClientRect(window_win32.window_handle, &rc);
//
//	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
//
//
//
//
//	target2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua), &pBlackBrush_);
//
//	D2D1_RECT_F layoutRect = D2D1::RectF(0.0f, 500.0f, settings.width / 3, settings.height);
//
//	target2d->BeginDraw();
//
//	target2d->SetTransform(D2D1::IdentityMatrix());
//
//	//target2d->Clear({1.0f, 1.0f, 1.0f, 0.5f});
//
//
//	target2d->DrawText(
//			lpText,        // The string to render.
//			cTextLength_,    // The string's length.
//			pTextFormat_,    // The text format.
//			layoutRect,       // The region of the window where the text will be rendered.
//			pBlackBrush_     // The brush used to draw the text.
//			);
//
//	target2d->EndDraw();
//}

void render_logo() {
	ID2D1Bitmap *bitmap;
	LoadBitmapFromFile((PCWSTR) L"textures/logoBrownBig.png", 640, 640, &bitmap);

	target2d->BeginDraw();
	target2d->SetTransform(D2D1::Matrix3x2F::Identity());
	target2d->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// retrieve the size of the bitmap.
	if (bitmap)
		D2D1_SIZE_F size = bitmap->GetSize();

	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(0.0f, 0.0f);

	// draw a bitmap.
	target2d->DrawBitmap(
			bitmap,
			D2D1::RectF(upperLeftCorner.x, upperLeftCorner.y, upperLeftCorner.x + settings.width, upperLeftCorner.y + settings.height),
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
			);



	HRESULT hr = target2d->EndDraw();

//	render_text();

	present();
}

HRESULT LoadBitmapFromFile(PCWSTR uri, UINT destinationWidth,UINT destinationHeight, ID2D1Bitmap **ppBitmap) {
	IWICImagingFactory *factory;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory), (LPVOID *) &factory);



	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	hr = factory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	if (SUCCEEDED(hr)) {
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr)) {

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = factory->CreateFormatConverter(&pConverter);

	}


	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
				);
	}

	if (SUCCEEDED(hr)) {

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = target2d->CreateBitmapFromWicBitmap(
				pConverter,
				NULL,
				ppBitmap
				);
	}

	if (pDecoder)
		pDecoder->Release();
	if (pSource)
		pSource->Release();
	//pStream->Release();
	if (pConverter)
		pConverter->Release();
	//pScaler->Release();

	return hr;
}
