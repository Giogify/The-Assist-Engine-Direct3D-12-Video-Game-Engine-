#pragma once
#include "d3dx12.h"
#include "Light.h"
#include "AssistMath.h"
#include <array>

// VertexData struct
namespace DSU {
	struct VertexData {
		struct {
			float x{};
			float y{};
			float z{};
		} pos;
		struct {
			float x{};
			float y{};
		} texcoord;
		struct {
			float x{};
			float y{};
			float z{};
		} norm;
	};
	struct PositionData {
		AssistMath::AMFLOAT3 pos{};
	};
	struct MaterialData {
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorEmissive;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorAmbient;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorDiffuse;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorSpecular;

		float specularPower{};
		int isTextured{ false };
		int padding0{};
		int padding1{};
	};
	struct VertexConstantBuffer {
		AssistMath::FAMMATRIX transform{};
		AssistMath::FAMMATRIX camera{};
		AssistMath::FAMMATRIX projection{};
		AssistMath::FAMMATRIX invtpose{};
	};
	struct PixelConstantBuffer {
		MaterialData mtl{};
		AssistMath::AMFLOAT4 eyePos{};
		AssistMath::AMFLOAT4 globalAmbient{};
		std::array<Light::LightData, Light::MAX_LIGHTS> lights{};
	};
	struct Position {
		AssistMath::FAMVECTOR translation{};
		AssistMath::FAMVECTOR rotation{};
		AssistMath::FAMVECTOR center{};
	};
	struct Speed {
		AssistMath::FAMVECTOR deltaTranslation{};
		AssistMath::FAMVECTOR deltaRotation{};
	};
	struct PipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RS;
	};
	struct ObjectDrawStaticMatrices {
		AssistMath::FAMMATRIX camera;
		AssistMath::FAMMATRIX projection;
	};
	struct SphereCollisionCheckData {
		AssistMath::FAMVECTOR center{};
		float radius{};
	};
}
