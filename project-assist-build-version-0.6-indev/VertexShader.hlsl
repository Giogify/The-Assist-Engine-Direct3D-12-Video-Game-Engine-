/*
/  ----------------------------------------------
/  VertexShader.hlsl
/
/  
/  ----------------------------------------------
*/

// Constant Buffer Structure
struct matrices {
	row_major matrix transform;
	row_major matrix camera;
	row_major matrix projection;
	row_major matrix inversetranspose;
};

ConstantBuffer<matrices> m : register(b0);

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
	//output.pos = mul(m.projection, mul(m.camera, mul(m.transform, float4(input.pos, 1.0f))));
	output.pos = mul(mul(mul(float4(input.pos, 1.0f), m.transform), m.camera), m.projection);
	output.posWS = mul(float4(input.pos, 1.0f), m.transform);

	// normalize the object's normals to fix the world transform skew
	output.normWS = mul(input.norm, (float3x3)m.inversetranspose);
	output.tex = input.tex;

	return output;
}