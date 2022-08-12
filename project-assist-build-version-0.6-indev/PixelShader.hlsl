/*
/  ----------------------------------------------
/  PixelShader.hlsl
/
/  
/  ----------------------------------------------
*/

#define MAX_LIGHTS 8

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// Material Struct (5 vectors)
struct Material {
	
	float4 emissive; //Ke

	float4 ambient; //Ka

	float4 diffuse; //Kd

	float4 specular; //Ks
	
	float specularPower; //Ns
	bool isTextured;
	int padding;
};

// Light Struct (5 vectors)
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

// Light Constant Buffer (47 vectors)
cbuffer pixelCBuffer : register(b0) {

	Material mtl : packoffset(c0);
	float4 eyePos : packoffset(c5);
	float4 globalAmbient : packoffset(c6);
	Light lights[MAX_LIGHTS] : packoffset(c7);

}

// Texture and Sampler
SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

// Lighting Result Struct
struct LightingResult {
	float4 diffuse;
	float4 specular;
};

// --Helper Functions--

// Diffuse
float4 doDiffuse(Light light, float3 L, float3 N) {
	float NdotL = max(0, dot(N, L));
	return light.color * NdotL;
}

// Specular
float4 doSpecular(Light light, float3 V, float3 L, float3 N) {

	// Phong Lighting
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));

	//Blinn-Phong Lighting
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.color * pow(NdotH, mtl.specularPower);
}

// Attentuation
float doAttenuation(Light light, float d) {
	return 1.0f / (light.constAtten + light.linAtten * d + light.quadAtten * d * d);
}

// Point Light
LightingResult doPointLight(Light light, float3 V, float4 P, float3 N) {
	LightingResult result;

	float3 L = (light.pos - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = doAttenuation(light, distance);

	result.diffuse = doDiffuse(light, L, N) * attenuation;
	result.specular = doSpecular(light, V, L, N) * attenuation;

	return result;
}

// Directional Light
LightingResult doDirectionalLight(Light light, float3 V, float4 P, float3 N) {
	LightingResult result;

	float3 L = -light.direction.xyz;

	result.diffuse = doDiffuse(light, L, N);
	result.specular = doSpecular(light, V, L, N);

	return result;
}

// Spot Light
float doSpotCone(Light light, float3 L) {
	float minCos = cos(light.spotAngle);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}
LightingResult doSpotLight(Light light, float3 V, float4 P, float3 N) {
	LightingResult result;

	float3 L = (light.pos - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = doAttenuation(light, distance);
	float spotIntensity = doSpotCone(light, L);

	result.diffuse = doDiffuse(light, L, N) * attenuation * spotIntensity;
	result.specular = doSpecular(light, V, L, N) * attenuation * spotIntensity;

	return result;
}

// Compute Lighting
LightingResult computeLighting(float4 P, float3 N) {
	float3 V = normalize(eyePos - P).xyz;

	LightingResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0} };

	[loop]
	for (int i = 0; i < MAX_LIGHTS; i++) {
		LightingResult result = { {0, 0, 0, 0}, {0, 0, 0, 0} };

		if (!lights[i].isEnabled) continue;

		switch (lights[i].type) {
		case DIRECTIONAL_LIGHT: {
			result = doDirectionalLight(lights[i], V, P, N);
		}
			break;
		case POINT_LIGHT: {
			result = doPointLight(lights[i], V, P, N);
		}
			break;
		case SPOT_LIGHT: {
			result = doSpotLight(lights[i], V, P, N);
		}
			break;
		}

		totalResult.diffuse += result.diffuse;
		totalResult.specular += result.specular;
	}

	totalResult.diffuse = saturate(totalResult.diffuse);
	totalResult.specular = saturate(totalResult.specular);

	return totalResult;
}

float3 toSRGB(float3 rgb) {
	return rgb < 0.0031308 ? 12.92 * rgb : 1.055 * pow(abs(rgb), 1.0 / 2.4) - 0.055;
}

// Input Struct
struct PS_INPUT {
	float2 tex : TEXCOORD0;
	float4 posWS : TEXCOORD1;
	float3 normWS : TEXCOORD2;
};

// Shader Main
float4 main(PS_INPUT input) : SV_TARGET0 {

	// Obtain the lighting data for that pixel based on local vertices and those vertices normals
	LightingResult lit = computeLighting(input.posWS, normalize(input.normWS));

	float4 emissive = mtl.emissive;
	float4 ambient = mtl.ambient * globalAmbient;
	float4 diffuse = mtl.diffuse * lit.diffuse;
	float4 specular = mtl.specular * lit.specular;

	//return Texture.Sample(Sampler, input.tex);

	//float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//if (mtl.isTextured) texColor = Texture.Sample(Sampler, input.tex);
	float4 texColor = Texture.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));

	
	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;

	return finalColor;

	//return float4(toSRGB(texColor.rgb), texColor.a);
	
	//float4 finalColor = texColor;
	
	// pixel color based on normal
	
	// normal color
	//return float4(input.normWS, 1.0f);

	// pastel color
	//return float4((input.normWS.x + 1.0f) / 2.0f,(input.normWS.y + 1.0f) / 2.0f,(input.normWS.z + 1.0f) / 2.0f, 1.0f);
}