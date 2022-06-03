/*
/  ----------------------------------------------
/  GraphicsOutput.h
/
/  This header / class file implements the Direct 
/  3D interfaces to configure a graphics pipeline 
/  and outputs it to the specified window handler.
/  
/  Currently supports a basic graphics pipeline 
/  with input assembly, vertex  and constant 
/  buffers, transformation matrices, vertex and 
/  pixel shaders, and simple rasterization.
/  ----------------------------------------------
*/
#pragma once
#include "TestException.h"
#include "d3dx12.h"
#include "Timer.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>
#include <memory>
#include <iostream>

#pragma comment(lib, "D3DCompiler.lib")

using namespace Microsoft::WRL;
namespace DX = DirectX;

class GraphicsOutput {

	friend class Application;

public:

	// VertexData struct
	struct VertexData {
		struct {
			float x;
			float y;
			float z;
		} pos;
		struct {
			float x;
			float y;
		} texcoord;
		struct {
			float x;
			float y;
			float z;
		} norm;
		struct {
			float r;
			float g;
			float b;
			float a;
		} color;
	};

private: // Private Fields

	// Graphics Configuration
	bool											mError{ false };
	bool											mDebug{ false };
	bool											mUseWARPAdapter{ false };
	bool											mVSync{ false };
	BOOL											mTearingSupport{ false };
	bool											mFullscreen{ false };
	UINT											mBackBufferCount{ 3u };
	UINT											mShaderDebugCompileFlags{ D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION };
	UINT											mShaderReleaseCompileFlags{ 0u };
	D3D_FEATURE_LEVEL								mFeatureLevel{ D3D_FEATURE_LEVEL_11_0 };

	// Debug
	ComPtr<ID3D12Debug5>							mpDebugController{};
	ComPtr<ID3D12InfoQueue1>						mpInfoQueue{};
	// Core
	ComPtr<IDXGIFactory7>							mpFactory{};
	ComPtr<ID3D12Device9>							mpDevice{};
	ComPtr<IDXGISwapChain4>							mpSwapChain{};
	// Adapters
	ComPtr<IDXGIAdapter4>							mpHardwareAdapter{};
	ComPtr<IDXGIAdapter4>							mpWARPAdapter{};
	// Commands
	std::vector<ComPtr<ID3D12CommandAllocator>>		mpCommandAllocator{};
	ComPtr<ID3D12GraphicsCommandList6>				mpCommandList{};
	ComPtr<ID3D12CommandQueue>						mpCommandQueue{};
	// Pipeline state
	ComPtr<ID3D12PipelineState>						mpPipelineState{};
	// Root Signature
	ComPtr<ID3D12RootSignature>						mpRootSignature{};
	ComPtr<ID3DBlob>								mpSignatureRootBlob{};
	ComPtr<ID3DBlob>								mpErrorBlob{};
	// Render Target View(s)
	std::vector<ComPtr<ID3D12Resource2>>			mpRenderTargets{};
	ComPtr<ID3D12DescriptorHeap>					mpRTVHeap{};
	UINT											mRTVHeapSize{};
	D3D12_VIEWPORT									mViewport{};
	D3D12_RECT										mScissorRc{};
	// Depth Stencil
	ComPtr<ID3D12Resource2>							mpDepthStencilTexture{};
	ComPtr<ID3D12DescriptorHeap>					mpDSVHeap{};
	UINT											mDSVHeapSize{};
	// Fence
	ComPtr<ID3D12Fence1>							mpFence{};
	HANDLE											mhFenceEvent{};
	UINT64											mFrameIndex{};
	std::vector<UINT64>								mFenceValues{};
	UINT64											mCurrentFenceValue{};
	// Shaders
	ComPtr<ID3DBlob>								mpVSBytecode{};
	ComPtr<ID3DBlob>								mpPSBytecode{};
	// Projection Matrix
	DirectX::XMMATRIX								mProjection{};
	// Misc
	RECT											mWindowRc{};
	HRESULT											mHR{};
	std::string										mLastType{};
	// Benchmarking
	float											sum{};
	UINT											runInstances{ 0u };
	Timer											timerBenchmark{};

public: // Public Methods

	// Constructor
	GraphicsOutput(HWND& hWnd);

	// Destructor
	~GraphicsOutput() = default;

	int parseGraphicsConfig() noexcept;
	int createDebugLayer() noexcept;
	int createInfoQueue() noexcept;
	int createFactory() noexcept;
	int enumerateAdapters() noexcept;
	int createDevice() noexcept;
	int createCommandQueue() noexcept;
	int checkTearingSupport() noexcept;
	int createSwapChain(const HWND&, const UINT, const UINT) noexcept;
	int createRTV() noexcept;
	int createCommandAllocatorAndList() noexcept;
	int createFence() noexcept;
	
	// Frame sync
	UINT64 signalFence(UINT64& fenceValue) noexcept;
	int waitFence(UINT64) noexcept;
	int flushGPU(UINT64& fenceValue) noexcept;

	void endFrame();
	void flushBackBufferColor(float r, float g, float b) noexcept;
	int doRender() noexcept;

	int resizeWindow(UINT, UINT) noexcept;
	int setFullscreen() noexcept;

	void setProjection(const DirectX::XMMATRIX& projection) noexcept { mProjection = projection; }
	DX::XMMATRIX& getProjection() noexcept { return mProjection; }
	ComPtr<ID3D12GraphicsCommandList6>& getCommandList() noexcept { return mpCommandList; }
	ComPtr<ID3D12Device9>& getDevice() noexcept { return mpDevice; }

	void doDebug(HRESULT& hr, const char* type, const char* element, const char* verb) {
		if (hr == S_OK) {
			std::cout << '[' << type << "] " << element << ' ' << verb << " Sucessfully!\n";
		}
		else {
			std::cout << '[' << type << "] " << element << ' ' << verb << " Unsucessfully!" << " [ERROR CODE] " << hr << '\n';
		}
		mLastType = type;
	}
	void doDebug(HRESULT& hr, const char* element, const char* verb) {
		if (hr == S_OK) {
			std::cout << '[' << mLastType << "] " << element << ' ' << verb << " Sucessfully!\n";
		}
		else {
			std::cout << '[' << mLastType << "] " << element << ' ' << verb << " Unsucessfully!" << " [ERROR CODE] " << hr << '\n';
		}
	}
	void doDebug(HRESULT& hr, const char* type, const char* element, const char* verb, int amount) {
		if (hr == S_OK) {
			std::cout << '[' << type << "] " << element << " #" << amount << ' ' << verb << " Sucessfully!\n";
		}
		else {
			std::cout << '[' << type << "] " << element << " #" << amount << ' ' << verb << " Unsucessfully!" << " [ERROR CODE] " << hr << '\n';
		}
		mLastType = type;
	}
	void doDebug(HRESULT& hr, const char* element, const char* verb, int amount) {
		if (hr == S_OK) {
			std::cout << '[' << mLastType << "] " << element << " #" << amount << ' ' << verb << " Sucessfully!\n";
		}
		else {
			std::cout << '[' << mLastType << "] " << element << " #" << amount << ' ' << verb << " Unsucessfully!" << " [ERROR CODE] " << hr << '\n';
		}
	}
	
};