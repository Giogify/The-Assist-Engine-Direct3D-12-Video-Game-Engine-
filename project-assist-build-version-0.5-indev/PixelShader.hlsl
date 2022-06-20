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

// Texture and Sampler
//Texture2D Texture : register(t0);
//sampler Sampler : register(s0);

// Material Struct (9 vectors)
struct Material {
	
	double4 emissive; //Ke

	double4 ambient; //Ka

	double4 diffuse; //Kd

	double4 specular; //Ks
	
	double specularPower; //Ns
	bool isTextured;
	int padding;
};

// Light Struct (9 vectors)
struct Light {
	
	double4 pos;
	
	double4 direction;

	double4 color;

	double spotAngle;
	double constAtten;

	double linAtten;
	double quadAtten;

	int type;
	bool isEnabled;
	int2 padding;

};

// Light Constant Buffer (47 vectors)
cbuffer pixelCBuffer : register(b0) {

	Material mtl : packoffset(c0);
	double4 eyePos : packoffset(c10);
	double4 globalAmbient : packoffset(c12);
	Light lights[MAX_LIGHTS] : packoffset(c14);

}

// Lighting Result Struct
struct LightingResult {
	double4 diffuse;
	double4 specular;
};

// ---Helper Functions---

// Diffuse
double4 doDiffuse(Light light, double3 L, double3 N) {
	double NdotL = max(0, dot(N, L));
	return light.color * NdotL;
}

// Specular
double4 doSpecular(Light light, double3 V, double3 L, double3 N) {

	// Phong Lighting
	double3 R = normalize(reflect(-L, N));
	double RdotV = max(0, dot(R, V));

	//Blinn-Phong Lighting
	double3 H = normalize(L + V);
	double NdotH = max(0, dot(N, H));

	return light.color * pow(NdotH, mtl.specularPower);
}

// Attentuation
double doAttenuation(Light light, double d) {
	return 1.0f / (light.constAtten + light.linAtten * d + light.quadAtten * d * d);
}

// Point Light
LightingResult doPointLight(Light light, double3 V, double4 P, double3 N) {
	LightingResult result;

	double3 L = (light.pos - P).xyz;
	double distance = length(L);
	L = L / distance;

	double attenuation = doAttenuation(light, distance);

	result.diffuse = doDiffuse(light, L, N) * attenuation;
	result.specular = doSpecular(light, V, L, N) * attenuation;

	return result;
}

// Directional Light
LightingResult doDirectionalLight(Light light, double3 V, double4 P, double3 N) {
	LightingResult result;

	double3 L = -light.direction.xyz;

	result.diffuse = doDiffuse(light, L, N);
	result.specular = doSpecular(light, V, L, N);

	return result;
}

// Spot Light
double doSpotCone(Light light, double3 L) {
	double minCos = cos(light.spotAngle);
	double maxCos = (minCos + 1.0f) / 2.0f;
	double cosAngle = dot(light.direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}
LightingResult doSpotLight(Light light, double3 V, double4 P, double3 N) {
	LightingResult result;

	double3 L = (light.pos - P).xyz;
	double distance = length(L);
	L = L / distance;

	double attenuation = doAttenuation(light, distance);
	double spotIntensity = doSpotCone(light, L);

	result.diffuse = doDiffuse(light, L, N) * attenuation * spotIntensity;
	result.specular = doSpecular(light, V, L, N) * attenuation * spotIntensity;

	return result;
}

// Compute Lighting
LightingResult computeLighting(double4 P, double3 N) {
	double3 V = normalize(eyePos - P).xyz;

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

// ---Helper Functions---

// Input Struct
struct PS_INPUT {
	float2 tex : TEXCOORD0;
	float4 posWS : TEXCOORD1;
	float3 normWS : TEXCOORD2;
};

// Shader Main
float4 main(PS_INPUT input) : SV_TARGET0 {

	LightingResult lit = computeLighting(input.posWS, normalize(input.normWS));

	double4 emissive = mtl.emissive;
	double4 ambient = mtl.ambient * globalAmbient;
	double4 diffuse = mtl.diffuse * lit.diffuse;
	double4 specular = mtl.specular * lit.specular;

	double4 texColor = double4(1.0f, 1.0f, 1.0f, 1.0f);

	//if (mtl.isTextured)	texColor = Texture.Sample(Sampler, input.tex);

	double4 finalColor = (emissive + ambient + diffuse + specular) * texColor;

	return finalColor;

}