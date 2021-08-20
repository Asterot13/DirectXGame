struct VS_INPUT
{
	float4 pos   : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 direction_to_camera : TEXCOORD2;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
	float4 m_camera_position;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.pos = lerp(input.pos, input.pos1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f);

	// World space
	output.pos = mul(input.pos, m_world);
	output.direction_to_camera = normalize(output.pos.xyz - m_camera_position.xyz);
	// View space
	output.pos = mul(output.pos, m_view);
	// Screen space
	output.pos = mul(output.pos, m_proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}