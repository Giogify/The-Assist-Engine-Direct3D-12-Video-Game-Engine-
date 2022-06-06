struct ProjectionMatrix {
	matrix m;
};
ConstantBuffer<ProjectionMatrix> ProjectionMatrixCB : register(b0);

struct VS_INPUT {
	float3 pos : POSITION;
	float3 color : COLOR;
};
struct VS_OUTPUT {
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	output.pos = mul(ProjectionMatrixCB.m, float4(input.pos, 1.0f));
	output.color = float4(input.color, 1.0f);
	return output;
}