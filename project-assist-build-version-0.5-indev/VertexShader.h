//#pragma once
//#include "GraphicsOutput.h"
//#include <iostream>
//
//class VertexShader {
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
//	VertexShader() = default;
//	VertexShader(const std::wstring& path) : mPath(path) {
//
//		ComPtr<ID3DBlob> pErrorBlob;
//		
//#if defined(_DEBUG)
//		HRESULT hr = D3DCompileFromFile(mPath.c_str(), nullptr, nullptr, "main", "vs_6_0",
//			mDebugCompileFlags, 0u, &mpBytecode, &pErrorBlob);
//#else
//		D3DCompileFromFile(path.c_str(), nullptr, nullptr, "main", "vs_6_5",
//			mReleaseCompileFlags, 0u, &mpBytecode, nullptr);
//#endif
//	}
//	
//	ComPtr<ID3DBlob>& getBytecode() noexcept { return mpBytecode; }
//
//};