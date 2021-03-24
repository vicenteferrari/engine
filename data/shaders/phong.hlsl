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
	float3 world_pos : WPOSITION;
	float3 view_pos : VPOSITION;
	float4 screen_pos : SV_POSITION;
	float3 w_norm : WNORMAL;
	float3 v_norm : VNORMAL;
	float2 tc : TEXCOORD;
};

Output main(Input input) {
	Output output;

	output.world_pos = (float3) mul(float4(input.pos, 1.0f), world);
	output.view_pos = (float3) mul(float4(input.pos, 1.0f), world_view);
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
	float4 screen_pos : SV_POSITION;
	float3 w_norm : WNORMAL;
	float3 v_norm : VNORMAL;
	float2 tc : TEXCOORD;
};

float4 main(Input input) : SV_TARGET {

	// fragment to light vector data
	const float3 vToL = light_view_pos - input.view_pos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	// attenuation
	const float att = 1.0f / (atten_const + atten_lin * distToL + atten_quad * (distToL * distToL));

	// diffuse intensity
	const float3 diffuse = diffuse_color * diffuse_intensity * att * max(0.0f, dot(dirToL, input.v_norm));

	// reflected light vector
	const float3 w = input.v_norm * dot(vToL, input.v_norm);
	const float3 r = w * 2.0f - vToL;

	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
	const float3 specular = att * diffuse_color * diffuse_intensity * specular_intensity * pow(max(0.0f, dot(normalize(-r),normalize(input.view_pos))), specular_power);

	// final color
	return float4(saturate((diffuse + ambient + specular) * material_color * tex.Sample(splr, input.tc)), 1.0f);
}

#endif
