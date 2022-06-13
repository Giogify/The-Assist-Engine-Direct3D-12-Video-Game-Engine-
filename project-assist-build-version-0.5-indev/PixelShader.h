//#pragma once
//#include "GraphicsOutput.h"
//
//class PixelShader {
//
//private:
//
//	static const UINT mDebugCompileFlags{ D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION };
//	static const UINT mReleaseCompileFlags{ 0u };
//
//	ComPtr<ID3DBlob> mpBytecode{};
//	std::wstring mPath{};
//
//public:
//
//	PixelShader() = default;
//	PixelShader(const std::wstring& path) : mPath(path) {
//		#if defined(_DEBUG)
//		D3DCompileFromFile(path.c_str(), nullptr, nullptr, "main", "ps_6_6",
//			mDebugCompileFlags, 0u, &mpBytecode, nullptr);
//		#else
//		D3DCompileFromFile(path.c_str(), nullptr, nullptr, "main", "ps_6_6",
//			mReleaseCompileFlags, 0u, &mpBytecode, nullptr);
//		#endif
//	}
//
//	ComPtr<ID3DBlob>& getBytecode() noexcept { return mpBytecode; }
//
//};