/*
/  ----------------------------------------------
/  PixelShader.hlsl
/
/  
/  ----------------------------------------------
*/

struct PS_INPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};
struct PS_OUTPUT {
	float4 color : SV_TARGET;
};

PS_OUTPUT main(PS_INPUT input) {

	PS_OUTPUT output;
	output.color = input.color;
	return output;
}