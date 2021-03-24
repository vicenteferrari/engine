#if defined(VERTEX)

cbuffer Transform {
	matrix world;
	matrix world_view;
	matrix world_view_proj;
};

struct Input {
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 tc : TEXCOORD;
};

struct Output {
	float3 w_pos : WPOSITION;
	float3 v_pos : VPOSITION;
	float4 screen_pos : SV_POSITION;
	float3 w_norm : WNORMAL;
	float3 v_norm : VNORMAL;
	float2 tc : TEXCOORD;
};

Output main(Input input) {
	Output output;

	output.w_pos = (float3) mul(float4(input.pos, 1.0f), world);
	output.v_pos = (float3) mul(float4(input.pos, 1.0f), world_view);
	output.screen_pos = mul(float4(input.pos, 1.0f), world_view_proj);

	output.w_norm = normalize(mul(input.norm, (float3x3) world));
	output.v_norm = normalize(mul(input.norm, (float3x3) world_view));

	output.tc = input.tc;

	return output;
}

#endif

#if defined(PIXEL)

cbuffer Light : register(b0) {
	float3 light_world_pos;
	float3 light_view_pos;
	float3 ambient;
	float3 diffuse_color;
	float diffuse_intensity;
	float atten_const;
	float atten_lin;
	float atten_quad;
};

cbuffer Material : register(b1) {
    float4 material_color;
	float specular_intensity;
	float specular_power;
};

Texture2D tex;
SamplerState splr;

struct Input {
	float3 world_pos : WPOSITION;
	float3 view_pos : VPOSITION;
	float4 pos : SV_POSITION;
	float3 w_norm : WNORMAL;
	float3 v_norm : VNORMAL;
	float2 tc : TEXCOORD;
};

float4 main(Input input) : SV_TARGET {
	float3 pixel_to_light = light_world_pos - input.world_pos;

	float dot_product = dot(normalize(input.w_norm), normalize(pixel_to_light));

	float intensity = 1.0f;

	if (dot_product <= 0.5f || dot_product >= 0.99f) {
		intensity = 0.2;
	}

	intensity *= diffuse_intensity;

	intensity /= length(pixel_to_light);

	return material_color * intensity;
}

#endif
