#pragma once
#include <DirectXMath.h>

// VertexData struct
namespace DataStructsUtil {
	struct VertexData {
		DirectX::XMFLOAT3 pos{};
		//DX::XMFLOAT2 texcoord{};
		//DX::XMFLOAT3 norm{};
		DirectX::XMFLOAT3 color{};
	};
	struct VertexData2D {
		DirectX::XMFLOAT2 pos{};
		//DX::XMFLOAT2 texcoord{};
		//DX::XMFLOAT3 norm{};
		DirectX::XMFLOAT3 color{};
	};
}
