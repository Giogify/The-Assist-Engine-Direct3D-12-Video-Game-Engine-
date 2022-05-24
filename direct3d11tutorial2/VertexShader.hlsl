/*
/  ----------------------------------------------
/  VertexShader.hlsl
/
/  
/  ----------------------------------------------
*/

// Constant Buffer Structure
cbuffer constantBuffer : register(b0) {

	matrix transform : packoffset(c0);
	matrix camera : packoffset(c4);
	matrix projection : packoffset(c8);
	matrix inversetranspose : packoffset(c12);
	
};

// Input Struct (must be like Input Layout)
struct VS_INPUT {

	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 norm : NORMAL0;
};

// Output Struct (must match Pixel Shader Input Struct)
struct VS_OUTPUT {
	float2 tex : TEXCOORD0;
	float4 posWS : TEXCOORD1;
	float3 normWS : TEXCOORD2;
	float4 pos : SV_POSITION;
};

// main
VS_OUTPUT main(VS_INPUT input) {

	// Create output object
	VS_OUTPUT output;

	// Positional coords -> world coords
	output.pos = mul(projection, mul(camera, mul(transform, float4(input.pos, 1.0f))));
	output.posWS = mul(transform, float4(input.pos, 1.0f));

	output.normWS = mul((float3x3)inversetranspose, input.norm);
	output.tex = input.tex;

	return output;
}