/*
/  ----------------------------------------------
/  VertexShader.hlsl
/
/  
/  ----------------------------------------------
*/

// Constant Buffer Structure
cbuffer constantBuffer : register(b0) {

	float4 Color_Diffuse			: packoffset(c0);
	float3 Color_Emissive			: packoffset(c1);
	float3 Color_Specular			: packoffset(c2);
	float Specular_Power			: packoffset(c2.w);

	float3 Light_Direction[3]		: packoffset(c3);
	float3 Light_Color_Diffuse[3]	: packoffset(c6);
	float3 Light_Color_Specular[3]	: packoffset(c9);

	float3 posEye					: packoffset(c12);

	float3 Fog_Color				: packoffset(c13);
	float4 Fog_Vector				: packoffset(c14);

	float4x4 transform				: packoffset(c15);
	float3x3 inversetranspose		: packoffset(c19);
	float4x4 camera					: packoffset(c22);
	float4x4 projection				: packoffset(c26);
	
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
	float4 diffuse : COLOR0;
	float4 posPS : SV_POSITION;
};

// main
VS_OUTPUT main(VS_INPUT input) {

	// Create output object
	VS_OUTPUT output;

	output.tex = input.tex;
	output.posWS = float4(input.pos, 1.0f);
	output.normWS = float3(1.0f, 1.0f, 1.0f);
	output.diffuse = float4(0.1f, 0.1f, 0.1f, 1.0f);
	output.posPS = mul(mul(mul(float4(input.pos, 1.0f), transform), camera), projection);

	// Positional coords -> world coords
	output.posPS = mul(mul(mul(float4(input.pos, 1.0f), transform), camera), projection);
	output.posWS.xyz = mul(float4(input.pos, 1.0f), transform).xyz;

	// Fog Factor
	output.posWS.w = saturate(dot(float4(input.pos, 1.0f), Fog_Vector));

	// 
	output.normWS = normalize(mul(input.norm, inversetranspose));
	output.diffuse = Color_Diffuse;
	output.tex = input.tex;

	return output;
}