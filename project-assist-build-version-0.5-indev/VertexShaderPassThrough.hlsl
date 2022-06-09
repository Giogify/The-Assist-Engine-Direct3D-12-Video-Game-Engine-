struct Matrices {
	matrix transform;
	matrix camera;
	matrix projection;
	matrix invtpose;
};

ConstantBuffer<Matrices> m;

struct VS_INPUT {
	float3 pos : POSITION0;
};

float4 main(VS_INPUT input) : SV_POSITION {
	return mul(m.projection, mul(m.camera, mul(m.transform, float4(input.pos, 1.0f))));
}