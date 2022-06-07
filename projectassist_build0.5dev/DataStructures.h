#pragma once
#include <DirectXMath.h>

// VertexData struct
namespace DataStructsUtil {
	struct Matrices {
		DirectX::XMMATRIX transform{};
		DirectX::XMMATRIX camera{};
		DirectX::XMMATRIX projection{};
		DirectX::XMMATRIX invtpose{};
	};
	struct VertexData {
		DirectX::XMFLOAT3 pos{};
		DirectX::XMFLOAT2 texcoord{};
		DirectX::XMFLOAT3 norm{};
		DirectX::XMFLOAT3 color{};
	};
	// Material Data
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
}
