#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Material {
	float4 emissive; //Ke
	float4 ambient; //Ka
	float4 diffuse; //Kd
	float4 specular; //Ks
	float specularPower; //Ns
	bool isTextured;
	int2 padding;
};
struct Light {
	float4 pos;
	float4 direction;
	float4 color;
	float spotAngle;
	float constAtten;
	float linAtten;
	float quadAtten;
	int type;
	bool isEnabled;
	int2 padding;
};
cbuffer pixelCB : register(b0) {
	Material mtl : packoffset(c0);
	float4 eyePos : packoffset(c5);
	float4 globalAmbient : packoffset(c6);
	Light lights[MAX_LIGHTS] : packoffset(c7);
}

struct PS_INPUT {
	float2 tex : TEXCOORD0;
	float4 posWS : TEXCOORD1;
	float3 normWS : TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_TARGET0 {
	return float4(input.posWS);
}