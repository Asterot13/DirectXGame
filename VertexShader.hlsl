struct VS_INPUT
{
	float4 pos   : POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.pos = lerp(input.pos, input.pos1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f);

	// World space
	output.pos = mul(input.pos, m_world);
	// View space
	output.pos = mul(output.pos, m_view);
	// Screen space
	output.pos = mul(output.pos, m_proj);

	output.color = input.color;
	output.color1 = input.color1;

	return output;
}