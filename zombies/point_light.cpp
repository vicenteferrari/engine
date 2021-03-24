//
// Created by vfs on 5/23/2020.
//

extern ID3D11Device *device;
extern GameState *current_state;
extern Memory memory;

PointLight::PointLight(GameState *state) {

	this->state = state;

	n_point_light = NPointLight();

	ambient = {0.05f, 0.05f, 0.05f};
	diffuse_color = {1.0f, 1.0f, 1.0f};
	atten_const = 1.0f;
	diffuse_intensity = 1.0f;
	atten_lin = 0.045f;
	atten_quad = 0.0075f;

	PointLightConstant constant = get_constant();

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = sizeof(constant);
	cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &constant;
	device->CreateBuffer(&cbd, &csd, &buffer.constantBuffer);

	// allocation
	model = Model(5, 4, 0, get_transform());
}

void PointLight::update() {
	// right now, we want to update the pointlight constant buffer every frame
	// to update the imgui bound data
	// remember that a point light owns a "PointLightConstantBuffer" which in turn
	// owns a buffer in GPU memory

	PointLightConstant constant = get_constant();

	update_pointlight_constant_buffer(&buffer, constant, sizeof(constant));

	Material mat = {{diffuse_color.x, diffuse_color.y, diffuse_color.z, 1.0f}, 0.6f, 30.0f};

	model.update(get_transform(), mat);
}

void PointLight::imgui() {
	if (ImGui::Begin("Point Light")) {
		ImGui::Text("Position");
		ImGui::SliderFloat3("Position", &n_point_light.position.x, -8.0f, 8.0f);

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &diffuse_intensity, 0.01f, 2.0f);
		ImGui::ColorEdit3("Diffuse Color", &diffuse_color.x);
		ImGui::ColorEdit3("Ambient Color", &ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &atten_const, 0.05f, 10.0f);
		ImGui::SliderFloat("Linear", &atten_lin, 0.0001f, 4.0f);
		ImGui::SliderFloat("Quadratic", &atten_quad, 0.0000001f, 10.0f);

		if (ImGui::Button("Reset")) {
			reset();
		}
	}
	ImGui::End();
}

void PointLight::render() {
	model.render();
}

void PointLight::bind() {
	bind_pointlight_constant_buffer(&buffer);
}

void PointLight::reset() {
	ambient = {0.05f, 0.05f, 0.05f};
	diffuse_color = {1.0f, 1.0f, 1.0f};
	atten_const = 1.0f;
	diffuse_intensity = 1.0f;
	atten_lin = 0.045f;
	atten_quad = 0.0075f;
}

PointLightConstant PointLight::get_constant() {
	auto world_view = this->state->camera.get_matrix();

	dx::XMFLOAT3 posf = {n_point_light.position.x, n_point_light.position.y, n_point_light.position.z};
	dx::XMVECTOR pos = dx::XMLoadFloat3(&posf);

	dx::XMVECTOR view_pos = dx::XMVector3Transform(pos, world_view);

	dx::XMFLOAT3 view_posf = {};
	dx::XMStoreFloat3(&view_posf, view_pos);

	PointLightConstant constant = {
			{n_point_light.position.x, n_point_light.position.y, n_point_light.position.z},
			view_posf,
			ambient,
			diffuse_color,
			diffuse_intensity,
			atten_const,
			atten_lin,
			atten_quad
	};

	return constant;
}

VertexShaderConstant PointLight::get_transform() {
	const VertexShaderConstant cb = {
			{
					dx::XMMatrixTranspose(
							dx::XMMatrixScaling(0.0625f, 0.0625f, 0.0625f) *
							dx::XMMatrixRotationRollPitchYaw(n_point_light.orientation.pitch, n_point_light.orientation.yaw, n_point_light.orientation.roll) *
							dx::XMMatrixTranslation(n_point_light.position.x, n_point_light.position.y, n_point_light.position.z)
					)
			},
			{
					{
							dx::XMMatrixTranspose(
									dx::XMMatrixScaling(0.0625f, 0.0625f, 0.0625f) *
									dx::XMMatrixRotationRollPitchYaw(n_point_light.orientation.pitch, n_point_light.orientation.yaw,
																	 n_point_light.orientation.roll) *
									dx::XMMatrixTranslation(n_point_light.position.x, n_point_light.position.y, n_point_light.position.z) *
									state->camera.get_matrix()
							)
					}
			},
			{
					dx::XMMatrixTranspose(
							dx::XMMatrixScaling(0.0625f, 0.0625f, 0.0625f) *
							dx::XMMatrixRotationRollPitchYaw(n_point_light.orientation.pitch, n_point_light.orientation.yaw, n_point_light.orientation.roll) *
							dx::XMMatrixTranslation(n_point_light.position.x, n_point_light.position.y, n_point_light.position.z) *
							state->camera.get_matrix() *
							dx::XMMatrixPerspectiveLH(1.0f, get_ar(), 0.5f, 100.0f)
					)
			}
	};

	return cb;
}

void PointLight::blend(float alpha, PointLight *blended, PointLight *previous, PointLight *current) {
	NPointLight::blend(alpha, &blended->n_point_light, &previous->n_point_light, &current->n_point_light);
}
