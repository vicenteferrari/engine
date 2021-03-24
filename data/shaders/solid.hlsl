#if defined(VERTEX)

cbuffer Transform {
	matrix world;
	matrix world_view;
	matrix world_view_proj;
};

struct Out {
	float3 world_pos : WPOSITION;
	float3 view_pos : VPOSITION;
	float4 screen_pos : SV_POSITION;
	float3 w_norm : WNORMAL;
	float3 v_norm : VNORMAL;
	float2 tex : TEXCOORD;
};

Out main(float3 pos : POSITION, float3 norm : NORMAL, float2 tex : TEXCOORD) {
	Out output;

	output.world_pos = (float3) mul(float4(pos, 1.0f), world);
	output.view_pos = (float3) mul(float4(pos, 1.0f), world_view);
	output.screen_pos = mul(float4(pos, 1.0f), world_view_proj);

	output.w_norm = mul(norm, (float3x3) world);
	output.v_norm = mul(norm, (float3x3) world_view);

	output.tex = tex;

	return output;
}

#endif

#if defined(PIXEL)

cbuffer Light : register(b0) {
	float3 light_pos;
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


	return material_color;
}

#endif
