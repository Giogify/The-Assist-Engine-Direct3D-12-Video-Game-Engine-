/*
/  ----------------------------------------------
/  GraphicsOutput.h
/
/  This header / class file implements the Direct 
/  3D interfaces to configure a graphics pipeline 
/  and outputs it to the specified window handle.
/  
/  Currently supports a basic graphics pipeline 
/  with input assembly, vertex  and constant 
/  buffers, transformation matrices, vertex and 
/  pixel shaders, and simple rasterization.
/  ----------------------------------------------
*/
#pragma once
#include "d3dx12.h"
#include "Timer.h"
#include "Camera.h"
#include "DataStructures.h"
#include "AssistMath.h"

#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include <thread>

#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib, "D3DCompiler.lib")

using namespace Microsoft::WRL;

class GraphicsOutput {

	friend class Application;
	friend class Window;

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
	bool											mWireframe{ false };

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
	DSU::PipelineStateStream						mPSS{};
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
	UINT8											mFrameIndex{};
	UINT64											mFenceValue{};
	UINT64											mCurrentFenceValue{};
	// Shaders
	ComPtr<ID3DBlob>								mpVSBytecode{};
	ComPtr<ID3DBlob>								mpPSBytecode{};
	// Projection Matrix
	AssistMath::FAMMATRIX							mProjection{};
	// Misc
	RECT											mWindowRc{};
	HRESULT											mHR{};
	std::string										mLastType{};
	Camera											mCamera{};

public: // Public Methods

	// Constructor
	GraphicsOutput(HWND& hWnd);

	// Destructor
	~GraphicsOutput() = default;

	void parseGraphicsConfig() noexcept;
	void createDebugLayer() noexcept;
	void createInfoQueue() noexcept;
	void createFactory() noexcept;
	void enumerateAdapters() noexcept;
	void createDevice() noexcept;
	void createCommandQueue() noexcept;
	void checkTearingSupport() noexcept;
	void createSwapChain(const HWND&, const UINT16&, const UINT16&) noexcept;
	void createRTV() noexcept;
	void createDSV(const UINT16&, const UINT16&) noexcept;
	void createCommandAllocatorAndList() noexcept;
	void createFence() noexcept;
	void initializePipeline() noexcept;

	// Frame sync
	void signalFence() noexcept;
	int waitFence() noexcept;
	int flushGPU() noexcept;
	
	// Prepare
	void transitionRTVToWrite() noexcept;
	void transitionRTVToRead() noexcept;
	void clearRTV() noexcept;
	void clearDSV() noexcept;
	void setRenderTarget() noexcept;

	// Render
	void startFrame() noexcept;
	void doFrame() noexcept;
	void endFrame() noexcept;

	// Util
	int resizeWindow(UINT, UINT) noexcept;
	int setFullscreen() noexcept;
	Camera& getCamera() noexcept { return mCamera; }

	void setProjection(const AssistMath::FAMMATRIX& projection) noexcept { mProjection = projection; }
	AssistMath::FAMMATRIX& getProjection() noexcept { return mProjection; }
	ComPtr<ID3D12GraphicsCommandList6>& getCommandList() noexcept { return mpCommandList; }
	ComPtr<ID3D12Device9>& getDevice() noexcept { return mpDevice; }

	void doDebug(HRESULT& hr, const char* type, const char* element, const char* verb) {
		if (hr == S_OK) {
			std::cout << '[' << type << "] " << element << ' ' << verb << " Successfully!\n";
		}
		else {
			std::cout << '[' << type << "] " << element << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
		}
		mLastType = type;
	}
	void doDebug(HRESULT& hr, const char* element, const char* verb) {
		if (hr == S_OK) {
			std::cout << '[' << mLastType << "] " << element << ' ' << verb << " Successfully!\n";
		}
		else {
			std::cout << '[' << mLastType << "] " << element << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
		}
	}
	void doDebug(HRESULT& hr, const char* type, const char* element, const char* verb, int amount) {
		if (hr == S_OK) {
			std::cout << '[' << type << "] " << element << " #" << amount << ' ' << verb << " Successfully!\n";
		}
		else {
			std::cout << '[' << type << "] " << element << " #" << amount << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
		}
		mLastType = type;
	}
	void doDebug(HRESULT& hr, const char* element, const char* verb, int amount) {
		if (hr == S_OK) {
			std::cout << '[' << mLastType << "] " << element << " #" << amount << ' ' << verb << " Successfully!\n";
		}
		else {
			std::cout << '[' << mLastType << "] " << element << " #" << amount << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
		}
	}
	
};