cbuffer Matrices : register(b0) {
	matrix transform : packoffset(c0);
	matrix camera : packoffset(c4);
	matrix projection : packoffset(c8);
	matrix invtpose : packoffset(c12);
};

struct VS_INPUT {
	float3 pos : POSITION0;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	output.pos = mul(projection, mul(camera, mul(transform, float4(input.pos, 1.0f))));
	return output;
}