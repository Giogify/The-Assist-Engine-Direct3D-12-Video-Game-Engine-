// Block size in each dimension
#define BLOCK_SIZE_X 8
#define BLOCK_SIZE_Y 8
#define BLOCK_SIZE_Z 1

// Width and height bitmask values for sampling
#define WIDTH_HEIGHT_EVEN 0
#define WIDTH_ODD_HEIGHT_EVEN 1
#define WIDTH_EVEN_HEIGHT_ODD 2
#define WIDTH_HEIGHT_ODD 3

// Metadata regarding the incoming texture to be mipped
struct TextureData {
	uint SrcMipLevel;
	uint NumMipLevels;
	uint SrcDimension;
	bool isSRGB;
	float2 TexelSize;
}; ConstantBuffer<TextureData> metadata : register(b0);

// The original texture to be mipped (the real input here)
Texture2D<float4> SrcMip : register(t0);

// Arrays of output mips (the real output here)
RWTexture2D<float4> DstMip0 : register(u0);
RWTexture2D<float4> DstMip1 : register(u1);
RWTexture2D<float4> DstMip2 : register(u2);
RWTexture2D<float4> DstMip3 : register(u3);

// Sampler for sampling the texel data
SamplerState Sampler : register(s0);

// Individual RGBA channels seperated into four arrays and accessed in a group-shared GPU memory block for each Compute Shader thread with which to interact.
groupshared float r[64];
groupshared float g[64];
groupshared float b[64];
groupshared float a[64];

// Input data regarding the current threading/wave/multi-threading data
struct CS_INPUT {
	uint3 GroupID : SV_GroupID;
	uint3 GroupThreadID : SV_GroupThreadID;
	uint3 DispatchThreadID : SV_DispatchThreadID;
	uint GroupIndex : SV_GroupIndex;
};

// SRGB normalize
float3 toSRGB(float3 rgb) {
	return rgb < 0.0031308 ? 12.92 * rgb : 1.055 * pow(abs(rgb), 1.0 / 2.4) - 0.055;
}

// Color op func
void colorSt(uint idx, float4 color) {
	r[idx] = color.r;
	g[idx] = color.g;
	b[idx] = color.b;
	a[idx] = color.a;
}
float4 colorLd(uint idx) {
	return float4(r[idx], g[idx], b[idx], a[idx]);
}
float4 colorPk(float4 color) {
	if (metadata.isSRGB) return float4(toSRGB(color.rgb), color.a);
	else return color;
}

[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z)]
void main(CS_INPUT input) {
	float4 src0 = (float4)0;

	switch (metadata.SrcDimension) {
		case WIDTH_HEIGHT_EVEN: {
			float2 UV = metadata.TexelSize * (input.DispatchThreadID.xy + 0.5);

			src0 = SrcMip.SampleLevel(Sampler, UV, metadata.SrcMipLevel);
		} break;
		case WIDTH_ODD_HEIGHT_EVEN: {
			float2 UV = metadata.TexelSize * (input.DispatchThreadID.xy + float2(0.25, 0.5));
			float2 offset = metadata.TexelSize * float2(0.5, 0.0);
			src0 = 0.5 * (SrcMip.SampleLevel(Sampler, UV, metadata.SrcMipLevel) 
						  + SrcMip.SampleLevel(Sampler, UV + offset, metadata.SrcMipLevel));
		} break;
		case WIDTH_EVEN_HEIGHT_ODD: {
			float2 UV = metadata.TexelSize * (input.DispatchThreadID.xy + float2(0.5, 0.25));
			float2 offset = metadata.TexelSize * float2(0.0, 0.5);
			src0 = 0.5 * (SrcMip.SampleLevel(Sampler, UV, metadata.SrcMipLevel) 
						  + SrcMip.SampleLevel(Sampler, UV + offset, metadata.SrcMipLevel));
		} break;
		case WIDTH_HEIGHT_ODD: {
			float2 UV = metadata.TexelSize * (input.DispatchThreadID.xy + float2(0.25, 0.25));
			float2 offset = metadata.TexelSize * 0.5;
			src0 = SrcMip.SampleLevel(Sampler, UV, metadata.SrcMipLevel);
			src0 += SrcMip.SampleLevel(Sampler, UV + float2(offset.x, 0.0), metadata.SrcMipLevel);
			src0 += SrcMip.SampleLevel(Sampler, UV + float2(0.0, offset.y), metadata.SrcMipLevel);
			src0 += SrcMip.SampleLevel(Sampler, UV + float2(offset.x, offset.y), metadata.SrcMipLevel);
			src0 *= 0.25;
		} break;
	}

	DstMip0[input.DispatchThreadID.xy] = colorPk(src0);
	if (metadata.NumMipLevels == 1) return;
	colorSt(input.GroupIndex, src0);
	GroupMemoryBarrierWithGroupSync();

	if ((input.GroupIndex & 0x9) == 0) {
		float4 src1 = colorLd(input.GroupIndex + 0x01);
		float4 src2 = colorLd(input.GroupIndex + 0x08);
		float4 src3 = colorLd(input.GroupIndex + 0x09);
		src0 = 0.25 * (src0 + src1 + src2 + src3);

		DstMip1[input.DispatchThreadID.xy / 2] = colorPk(src0);
		colorSt(input.GroupIndex, src0);
	}

	if (metadata.NumMipLevels == 2) return;
	GroupMemoryBarrierWithGroupSync();

	if ((input.GroupIndex & 0x1B) == 0) {
		float4 src1 = colorLd(input.GroupIndex + 0x02);
		float4 src2 = colorLd(input.GroupIndex + 0x10);
		float4 src3 = colorLd(input.GroupIndex + 0x12);
		src0 = 0.25 * (src0 + src1 + src2 + src3);

		DstMip2[input.DispatchThreadID.xy / 4] = colorPk(src0);
		colorSt(input.GroupIndex, src0);
	}

	if (metadata.NumMipLevels == 3) return;
	GroupMemoryBarrierWithGroupSync();

	if (input.GroupIndex == 0) {
		float4 src1 = colorLd(input.GroupIndex + 0x04);
		float4 src2 = colorLd(input.GroupIndex + 0x20);
		float4 src3 = colorLd(input.GroupIndex + 0x24);
		src0 = 0.25 * (src0 + src1 + src2 + src3);

		DstMip3[input.DispatchThreadID.xy / 8] = colorPk(src0);
	}
	
	return;
}