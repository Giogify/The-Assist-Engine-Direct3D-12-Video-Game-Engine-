cbuffer Matrices : register(b0) {
	matrix transform : packoffset(c0);
	matrix camera : packoffset(c4);
	matrix projection : packoffset(c8);
	matrix invtpose : packoffset(c12);
};

struct VS_INPUT {
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL0;
};

struct VS_OUTPUT {
	float2 tex : TEXCOORD0;
	float4 posWS : TEXCOORD1;
	float3 normWS : TEXCOORD2;
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	output.pos = mul(projection, mul(camera, mul(transform, float4(input.pos, 1.0f))));
	output.posWS = mul(transform, float4(input.pos, 1.0f));
	output.normWS = mul((float3x3)invtpose, input.norm);
	output.tex = input.tex;

	return output;
}