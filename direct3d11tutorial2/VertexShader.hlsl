/*
/  ----------------------------------------------
/  VertexShader.hlsl
/
/  
/  ----------------------------------------------
*/

// Custom Vertex Structs
struct VS_INPUT {
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
	float4 color : COLOR;
};
struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
	float3 norm : NORMAL;

};

// Constant Buffer Structure
cbuffer constantBuffer {
	matrix transform;
};

// main
VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	output.pos = mul(transform, float4(input.pos.x, input.pos.y, input.pos.z, 1.0f));
	output.tex = input.tex;
	output.norm = input.norm;
	output.color = input.color;
	return output;
}