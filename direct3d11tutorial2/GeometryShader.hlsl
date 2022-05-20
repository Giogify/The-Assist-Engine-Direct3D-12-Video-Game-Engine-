struct GS_INPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
	float3 norm : NORMAL;
};
struct GS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
	float3 norm : NORMAL;
};

[maxvertexcount(3)]
void main(triangle float4 input[3] : SV_POSITION, inout TriangleStream<GS_OUTPUT> output) {
	for (uint i = 0; i < 3; i++) {
		GS_INPUT input2;
		GS_OUTPUT element;
		element.pos = input[i];
		output.Append(element);
	}
}