struct VS_INPUT
{
	float4 pos   : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binoraml : BINORMAL0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	return output;
}