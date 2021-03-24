//
// Created by vfs on 5/27/2020.
//

extern MeshManager mesh_manager;
extern ShaderManager shader_manager;
extern TextureManager texture_manager;

Zombie::Zombie() {

}

Zombie::Zombie(GameState *state) {
	this->state = state;

	n_zombie = NZombie();

	// allocation
	model = Model(6, 0, 0, get_transform());
}

void Zombie::update(float dt) {
	n_zombie.update(dt);

	// pass it a world matrix made from the entity data
	// TODO: is this correct?
	model.update(get_transform());
}

void Zombie::render() {
	model.render();
}

VertexShaderConstant Zombie::get_transform() {
	const VertexShaderConstant cb = {
			{
					dx::XMMatrixTranspose(
							dx::XMMatrixRotationRollPitchYaw(n_zombie.orientation.pitch, n_zombie.orientation.yaw, n_zombie.orientation.roll) *
							dx::XMMatrixTranslation(n_zombie.position.x, n_zombie.position.y, n_zombie.position.z)
					)
			},
			{
					dx::XMMatrixTranspose(
							dx::XMMatrixRotationRollPitchYaw(n_zombie.orientation.pitch, n_zombie.orientation.yaw, n_zombie.orientation.roll) *
							dx::XMMatrixTranslation(n_zombie.position.x, n_zombie.position.y, n_zombie.position.z) *
							state->camera.get_matrix()
					)
			},
			{
					dx::XMMatrixTranspose(
							dx::XMMatrixRotationRollPitchYaw(n_zombie.orientation.pitch, n_zombie.orientation.yaw, n_zombie.orientation.roll) *
							dx::XMMatrixTranslation(n_zombie.position.x, n_zombie.position.y, n_zombie.position.z) *
							state->camera.get_matrix() *
							dx::XMMatrixPerspectiveLH(1.0f, get_ar(), 0.5f, 100.0f)
					)
			}
	};

	return cb;
}

void Zombie::imgui() {
	if (ImGui::Begin("Zombie")) {
		ImGui::Text("Transform");
		ImGui::SliderFloat3("Position", &n_zombie.position.x, -8.0f, 8.0f);
		ImGui::SliderFloat3("Orientation", &n_zombie.orientation.x, -8.0f, 8.0f);
		ImGui::SliderFloat3("Angular acceleration", &n_zombie.angular.x, -8.0f, 8.0f);

		//		ImGui::Text("Position");
		//		ImGui::SliderFloat("X", &position.x, -8.0f, 8.0f, "%.2f");
		//		ImGui::SliderFloat("Y", &position.y, -8.0f, 8.0f, "%.2f");
		//		ImGui::SliderFloat("Z", &position.z, -8.0f, 8.0f, "%.2f");

		ImGui::Text("Model");

		ImGui::Text("Mesh");
		for (u64 i = 0; i < mesh_manager.meshes_curr; ++i) {
			sprintf_s(meshes[i], "#%lld %s", i, mesh_manager.get_mesh(i)->name);
		}

		if (ImGui::BeginCombo("Mesh", meshes[model.mesh_id])) {
			for (int i = 0; i < mesh_manager.meshes_curr; ++i) {
				const bool is_selected = model.mesh_id == i;
				if (ImGui::Selectable(meshes[i], is_selected)) {
					model.mesh_id = i;
					model.mesh = mesh_manager.get_mesh(i);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Texture");
		for (u64 i = 0; i < texture_manager.textures_curr; ++i) {
			sprintf_s(textures[i], "#%lld %s", i, texture_manager.get_texture_by_id(i)->name);
		}

		if (ImGui::BeginCombo("Texture", textures[model.texture_id])) {
			for (int i = 0; i < texture_manager.textures_curr; ++i) {
				const bool is_selected = model.texture_id == i;
				if (ImGui::Selectable(textures[i], is_selected)) {
					model.texture_id = i;
					model.texture = texture_manager.get_texture_by_id(i);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Shader");
		for (u64 i = 0; i < shader_manager.shaders_curr; ++i) {
			Shader *shader = shader_manager.get_shader_by_id(i);
			if (shader)
				sprintf_s(shaders[i], "#%lld %s", i, shader->name);
			else
				sprintf_s(shaders[i], "#%lld %s", i, "compilation error");
		}

		if (ImGui::BeginCombo("Shader", shaders[model.shader_id])) {
			for (u64 i = 0; i < shader_manager.shaders_curr; ++i) {
				const bool is_selected = model.shader_id == i;
				if (ImGui::Selectable(shaders[i], is_selected)) {
					model.set_shader(i);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Material");
		ImGui::SliderFloat("Specular Intensity", &model.material.specular_intensity, 0.0f, 10.0f);
		ImGui::SliderFloat("Specular Power", &model.material.specular_power, 0.0f, 50.0f);
		ImGui::ColorEdit3("Color", &model.material.color.x);

		if (ImGui::Button("Reset")) {
			//reset();
		}
	}
	ImGui::End();
}

void Zombie::blend(float alpha, Zombie *blended, Zombie *previous, Zombie *current) {
	NZombie::blend(alpha, &blended->n_zombie, &previous->n_zombie, &current->n_zombie);
}
