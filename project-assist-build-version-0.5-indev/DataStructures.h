#pragma once
#include "d3dx12.h"
#include "Light.h"
#include "AssistMath.h"
#include <array>

// VertexData struct
namespace DSU {
	struct VertexData {
		AssistMath::AMDOUBLE3 pos{};
		AssistMath::AMDOUBLE2 texcoord{};
		AssistMath::AMDOUBLE3 norm{};
	};
	struct PositionData {
		AssistMath::AMDOUBLE3 pos{};
	};
	struct MaterialData {
		struct {
			double r{};
			double g{};
			double b{};
			double a{};
		} colorEmissive;
		struct {
			double r{};
			double g{};
			double b{};
			double a{};
		} colorAmbient;
		struct {
			double r{};
			double g{};
			double b{};
			double a{};
		} colorDiffuse;
		struct {
			double r{};
			double g{};
			double b{};
			double a{};
		} colorSpecular;

		double specularPower{};
		int isTextured{ false };
		int padding0{};
		int padding1{};
	};
	struct VertexConstantBuffer {
		AssistMath::AMMATRIX transform{};
		AssistMath::AMMATRIX camera{};
		AssistMath::AMMATRIX projection{};
		AssistMath::AMMATRIX invtpose{};
	};
	struct PixelConstantBuffer {
		MaterialData mtl{};
		AssistMath::AMDOUBLE4 eyePos{};
		AssistMath::AMDOUBLE4 globalAmbient{};
		std::array<Light::LightData, Light::MAX_LIGHTS> lights{};
	};
	struct Position {
		double x{};
		double y{};
		double z{};
		double pitch{};
		double yaw{};
		double roll{};
		AssistMath::AMDOUBLE3 center{};
	};
	struct Speed {
		double dx{};
		double dy{};
		double dz{};
		double dpitch{};
		double dyaw{};
		double droll{};
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
		AssistMath::AMMATRIX camera;
		AssistMath::AMMATRIX projection;
	};
	struct SphereCollisionCheckData {
		AssistMath::AMDOUBLE3 center{};
		double radius{};
	};
}
