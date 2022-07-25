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

StructuredBuffer<float3> pos_arr : register(t0);
StructuredBuffer<float2> tex_arr : register(t1);
StructuredBuffer<float3> norm_arr : register(t2);

// Input Struct (must be like Input Layout)
struct VS_INPUT {
	uint pos_idx : POSITION0;
	uint tex_idx : TEXCOORD0;
	uint norm_idx : NORMAL0;
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

	float3 posVert = pos_arr[input.pos_idx];
	float2 texVert = tex_arr[input.tex_idx];
	float3 normVert = norm_arr[input.norm_idx];

	// Positional coords -> world coords
	//output.pos = mul(m.projection, mul(m.camera, mul(m.transform, float4(input.pos, 1.0f))));
	output.pos = mul(mul(mul(float4(posVert, 1.0f), m.transform), m.camera), m.projection);
	output.posWS = mul(float4(posVert, 1.0f), m.transform);

	// normalize the object's normals to fix the world transform skew
	output.normWS = mul(normVert, (float3x3)m.inversetranspose);
	output.tex = texVert;

	return output;
}