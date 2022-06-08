#pragma once
#include "d3dx12.h"
#include "Light.h"
#include <array>
#include <DirectXMath.h>

// VertexData struct
namespace DataStructsUtil {
	struct VertexData {
		DirectX::XMFLOAT3 pos{};
		DirectX::XMFLOAT2 texcoord{};
		DirectX::XMFLOAT3 norm{};
	};
	struct PositionData {
		DirectX::XMFLOAT3 pos{};
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
		DirectX::XMMATRIX transform{};
		DirectX::XMMATRIX camera{};
		DirectX::XMMATRIX projection{};
		DirectX::XMMATRIX invtpose{};
	};
	struct PixelConstantBuffer {
		MaterialData mtl{};
		DirectX::XMFLOAT4 eyePos{};
		DirectX::XMFLOAT4 globalAmbient{};
		std::array<Light::LightData, Light::MAX_LIGHTS> lights{};
	};
	struct Position {
		float x{};
		float y{};
		float z{};
		float roll{};
		float pitch{};
		float yaw{};
	};
	struct Speed {
		float droll{};
		float dpitch{};
		float dyaw{};
		float dtheta{};
		float dphi{};
		float dchi{};
	};
	struct PipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
	};
}
