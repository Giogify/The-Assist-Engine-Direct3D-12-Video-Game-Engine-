/*
/  ----------------------------------------------
/  PixelShader.hlsl
/
/  
/  ----------------------------------------------
*/

// Constant Buffer Structure

Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

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

	matrix worldViewTransform		: packoffset(c15);
};

// Input Struct
struct PS_INPUT {
	float2 tex : TEXCOORD0;
	float4 posWS : TEXCOORD1;
	float3 normWS : TEXCOORD2;
	float4 diffuse : COLOR0;
};

struct ColorPair {
	float3 diffuse;
	float3 specular;
};

ColorPair computeLights(float3 eyeVector, float3 worldNorm, uniform int numLights) {
	float3x3 lightDirections = 0;
	float3x3 lightDiffuse = 0;
	float3x3 lightSpecular = 0;
	float3x3 halfVectors = 0;

	[unroll]
	for (int i = 0; i < numLights; i++) {
		lightDirections[i] = Light_Direction[i];
		lightDiffuse[i] = Light_Color_Diffuse[i];
		lightSpecular[i] = Light_Color_Specular[i];

		halfVectors[i] = normalize(eyeVector - lightDirections[i]);
	}

	float3 dotL = mul(-lightDirections, worldNorm);
	float3 dotH = mul(halfVectors, worldNorm);

	float3 zeroL = step(0, dotL);

	float3 diffuse = zeroL * dotL;
	float3 specular = pow(max(dotH, 0) * zeroL, Specular_Power) * dotL;

	ColorPair result;

	result.diffuse = mul(diffuse, lightDiffuse) * Color_Diffuse.rgb + Color_Emissive;
	result.specular = mul(specular, lightSpecular) * Color_Specular;

	return result;
}

float4 main(PS_INPUT input) : SV_TARGET0 {
	
	float4 color = Texture.Sample(Sampler, input.tex) * input.diffuse;
	float3 eyeVector = normalize(posEye - input.posWS.xyz);
	float3 worldNorm = normalize(input.normWS);
	ColorPair lightResult = computeLights(eyeVector, worldNorm, 3);
	color.rgb *= lightResult.diffuse;

	// Specular
	color.rgb += lightResult.specular * color.a;

	// Fog
	color.rgb = lerp(color.rgb, Fog_Color * color.a, input.posWS.w);

	return color;
}