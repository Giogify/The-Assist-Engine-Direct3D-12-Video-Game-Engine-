#include "GraphicsOutput.h"
#include <ios>
#include <sstream>
#include <d3dcompiler.h>
#include <array>
#include <fstream>
#include <thread>
#include <functional>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "DXGI.lib")

namespace wrl = Microsoft::WRL;
namespace DX = DirectX;

GraphicsOutput::GraphicsOutput(HWND& hWnd) {

	WINDOWINFO wndInfo = {};
	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);

	UINT width = wndInfo.rcClient.right - wndInfo.rcClient.left;
	UINT height = wndInfo.rcClient.bottom - wndInfo.rcClient.top;

#if defined(_DEBUG)
	mDebug = true;
#endif

	if (parseGraphicsConfig() != 0) mError = true;

	mFenceValues.resize(mBackBufferCount);

	if (mDebug) {
		std::cout << "[Graphics Pipeline | Initialization]\n";
		std::cout << "[Init Info] Using Debug Compile Flags for Shaders\n";
		std::cout << "[Init Info] Back Buffer Count: #" << mBackBufferCount << '\n';
		if (!mUseWARPAdapter) std::cout << "[Init Info] Expected Adapter Type: Hardware\n";
		if (mUseWARPAdapter) std::cout << "[Init Info] Expected Adapter Type: WARP\n";
		std::cout << "[Init Info] Minimum Feature Level: 11_0\n";
	}

	std::thread thread0([this] { createFactory(); });
	std::thread thread1([this] { createDebugLayer(); });
	thread0.join();
	std::thread thread2([this] { enumerateAdapters(); });
	thread2.join();
	std::thread thread3([this] { createDevice(); });
	thread3.join();
	std::thread thread4([this] { createCommandQueue(); });
	std::thread thread5([this] { checkTearingSupport(); });
	std::thread thread6([this] { createFence(); });
	std::thread thread7([this] { createInfoQueue(); });
	thread4.join();
	std::thread thread8([this, hWnd, width, height] { createSwapChain(hWnd, width, height); });
	std::thread thread9([this] { createCommandAllocatorAndList(); });
	thread8.join();
	std::thread thread10([this] { createRTV(); });
	thread1.join();thread5.join();thread6.join();thread7.join();thread9.join();thread10.join();

	/*if (createDebugLayer() != 0) mError = true;
	if (createFactory() != 0) mError = true;
	if (enumerateAdapters() != 0) mError = true;
	if (createDevice() != 0) mError = true;
	if (createCommandQueue() != 0) mError = true;
	if (checkTearingSupport() != 0) mError = true;
	if (createSwapChain(hWnd, width, height) != 0) mError = true;
	if (createRTV() != 0) mError = true;
	if (createCommandAllocatorAndList() != 0) mError = true;
	if (createFence() != 0) mError = true;*/
	//// Create the Vertex Shader and Pixel Shader
	//mHR = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_1", 0u, mShaderDebugCompileFlags, &mpVSBytecode, nullptr);
	//doDebug(mHR, "Shaders", "Vertex Shader", "Compiled");
	//mHR = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_1", 0u, mShaderDebugCompileFlags, &mpPSBytecode, nullptr);
	//doDebug(mHR, "Pixel Shader", "Compiled");
	//// Prepare the Shaders
	//std::array<CD3DX12_DESCRIPTOR_RANGE1, 2u> dr{};
	//dr[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1u, 0u, 0u); // ps register t0
	//dr[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1u, 1u, 0u); // ps register s0
	//std::array<CD3DX12_ROOT_PARAMETER1, 4u> rp{};
	//rp[0].InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX); // vs b0
	//rp[1].InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_PIXEL); // ps b0
	//rp[2].InitAsDescriptorTable(1u, &dr[0], D3D12_SHADER_VISIBILITY_PIXEL); // ps t0
	//rp[3].InitAsDescriptorTable(1u, &dr[1], D3D12_SHADER_VISIBILITY_PIXEL); // ps s0
	//CD3DX12_STATIC_SAMPLER_DESC ssd{};
	//ssd.Init(0u);
	//// Create the Root Signature
	//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC vrsd{};
	//vrsd.Init_1_1(4u, rp.data(), 1u, &ssd, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//mHR = D3D12SerializeVersionedRootSignature(&vrsd, &mpSignatureRootBlob, &mpErrorBlob);
	//doDebug(mHR, "Root Signature", "Root Signature", "Deserialized");
	//mHR = mpDevice->CreateRootSignature(0u, mpSignatureRootBlob->GetBufferPointer(), mpSignatureRootBlob->GetBufferSize(), IID_PPV_ARGS(&mpRootSignature));
	//doDebug(mHR, "Root Signature", "Created");
	//// Define the Input Layout
	//std::array<D3D12_INPUT_ELEMENT_DESC, 3> ied{};
	//ied[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	//ied[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	//ied[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	//// Create the Pipeline State Object
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC psod = {};
	//psod.InputLayout = { ied.data(), (UINT)ied.size() };
	//psod.pRootSignature = mpRootSignature.Get();
	//psod.VS = CD3DX12_SHADER_BYTECODE(mpVSBytecode.Get());
	//psod.PS = CD3DX12_SHADER_BYTECODE(mpPSBytecode.Get());
	//psod.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psod.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//psod.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
	//psod.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//psod.SampleMask = UINT_MAX;
	//psod.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//psod.NumRenderTargets = 1u;
	//psod.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	//psod.SampleDesc.Count = 1u;
	//mHR = mpDevice->CreateGraphicsPipelineState(&psod, IID_PPV_ARGS(&mpPipelineState));
	//doDebug(mHR, "Pipeline State", "PSO", "Bound");
	//// Configure Depth Stencil Texture
	//CD3DX12_HEAP_PROPERTIES DSTexHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	//auto DSTexHeapDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height);
	//DSTexHeapDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//mHR = mpDevice->CreateCommittedResource(&DSTexHeapProperties, D3D12_HEAP_FLAG_NONE, &DSTexHeapDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, nullptr,
	//	IID_PPV_ARGS(&mpDepthStencilTexture));
	//doDebug(mHR, "Depth Stencil", "Depth Stencil Texture", "Created");
	//// Create the Depth Stencil
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvd{};
	//dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	//dsvd.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Flags = D3D12_DSV_FLAG_NONE;
	//dsvd.Texture2D.MipSlice = 0u;
	//D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc = {};
	//DSVHeapDesc.NumDescriptors = 1u;
	//DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//mHR = mpDevice->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&mpDSVHeap));
	//doDebug(mHR,"Depth Stencil", "DSV Heap", "Allocated");
	//mDSVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//doDebug(mHR, "DSV Heap Size", "Fetched");
	//CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
	//mpDevice->CreateDepthStencilView(mpDepthStencilTexture.Get(), &dsvd, hDSV);
	//doDebug(mHR, "Depth Stencil View", "Created");
	//// Create the viewport
	//mViewport.Width = width;
	//mViewport.Height = height;
	//mViewport.MinDepth = 0.f;
	//mViewport.MaxDepth = 1.f;
	//mViewport.TopLeftX = 0.f;
	//mViewport.TopLeftY = 0.f;
	//// Create the Scissor Rectangle
	//mScissorRc = wndInfo.rcClient;
	// Generate View of the Depth Stencil Texture
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	//depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilViewDesc.Texture2D.MipSlice = 0u;
	//mpDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), &depthStencilViewDesc, &m_pDepthStencilView);
	//m_pDeviceContext->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView* const*>(m_pRenderTargetView.GetAddressOf()), m_pDepthStencilView.Get());
	//
	//// Configure Viewport
	//mViewport.Width = width;
	//mViewport.Height = height;
	//mViewport.MinDepth = 0;
	//mViewport.MaxDepth = 1;
	//mViewport.TopLeftX = 0;
	//mViewport.TopLeftY = 0;
	//mpFence->Set
	//m_pDeviceContext->RSSetViewports(1u, &vp);
	//// Create Sampler
	////auto samplerBorderColor = std::make_unique<float[]>(4);
	////for (int i = 0; i < 4; i++) samplerBorderColor[i] = 1.0f;
	//D3D12_SAMPLER_DESC samplerDesc = {};
	//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//samplerDesc.MinLOD = -FLT_MAX;
	//samplerDesc.MaxLOD = FLT_MAX;
	//samplerDesc.MipLODBias = 1.0f;
	//samplerDesc.MaxAnisotropy = 1u;
	//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//wrl::ComPtr<ID3D12SamplerState> samplerState;
	//mpDevice->CreateSampler(&samplerDesc, samplerState.GetAddressOf());
	//mpCommandList->PSSetSamplers(0u, 1u, samplerState.GetAddressOf());
}

// --Creation Methods--
int GraphicsOutput::parseGraphicsConfig() noexcept {

	enum GFXConfigDataType {
		UIBackBufferCount,
		BUseWARPAdapter,
		BVSync,
		BFullScreenOnStartup,
		Invalid
	};
	if (mDebug) std::cout << "[CONFIG] Parsing Graphics Configuration File...\n";
	std::string strCurrentLine{}; std::ifstream file{};
	file.open("gfx_config.txt");
	if (file.is_open()) { while (file) {
			std::getline(file, strCurrentLine);
			if (strCurrentLine.empty()) break;
			GFXConfigDataType currentData;
			if (strCurrentLine.starts_with("UIBackBufferCount")) currentData = UIBackBufferCount;
			else if (strCurrentLine.starts_with("BUseWARPAdapter")) currentData = BUseWARPAdapter;
			else if (strCurrentLine.starts_with("BVSync")) currentData = BVSync;
			else if (strCurrentLine.starts_with("BFullScreenOnStartup")) currentData = BFullScreenOnStartup;
			else currentData = Invalid;
			switch (currentData) {
			case UIBackBufferCount:
				strCurrentLine.erase(0u, 19u);
				mBackBufferCount = std::stoi(strCurrentLine);
				break;
			case BUseWARPAdapter:
				strCurrentLine.erase(0u, 17u);
				if (strCurrentLine == "TRUE") mUseWARPAdapter = true;
				break;
			case BVSync:
				strCurrentLine.erase(0u, 8u);
				if (strCurrentLine == "TRUE") mVSync = true;
				break;
			case BFullScreenOnStartup:
				strCurrentLine.erase(0u, 22u);
				if (strCurrentLine == "TRUE") mFullscreen = true;
				break;
			case Invalid:
				if (mDebug) std::cout << "[WARNING] Invalid line parsed from \"gfx_config.txt\"\n"
					<< "[LINE] " << strCurrentLine;
				break;
			}
			strCurrentLine.clear();
		}
	}
	return 0;
}
int GraphicsOutput::createDebugLayer() noexcept {
	// Create the Debug Interface
	if (mDebug) {
		D3D12GetDebugInterface(IID_PPV_ARGS(&mpDebugController));
		mpDebugController->EnableDebugLayer();
	}
	return 0;
}
int GraphicsOutput::createInfoQueue() noexcept {
	if (mDebug) {
		mpDevice.As(&mpInfoQueue);
		mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, TRUE);
	}
	return 0;
}
int GraphicsOutput::createFactory() noexcept {
	
	// Create factory
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mpFactory));
	return 0;
}
int GraphicsOutput::enumerateAdapters() noexcept {

	// Obtain adapter
	if (mUseWARPAdapter) {
		mpFactory->EnumWarpAdapter(IID_PPV_ARGS(&mpWARPAdapter)); // WARP
	}
	else {
		SIZE_T maxVideoMem = 0;
		ComPtr<IDXGIAdapter1> refAdapter{};
		for (UINT i = 0; mpFactory->EnumAdapters1(i, refAdapter.GetAddressOf()); i++) {
			DXGI_ADAPTER_DESC1 ad{};
			refAdapter->GetDesc1(&ad);
			if ((ad.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && D3D12CreateDevice(refAdapter.Get(), mFeatureLevel, __uuidof(ID3D12Device), nullptr)
				&& ad.DedicatedVideoMemory > maxVideoMem) {
				maxVideoMem = ad.DedicatedVideoMemory;
				refAdapter.As(&mpHardwareAdapter);
			}
		}
	}
	return 0;
}
int GraphicsOutput::createDevice() noexcept {

	// Create the Device
	if (mUseWARPAdapter) D3D12CreateDevice(mpWARPAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mpDevice)); // WARP
	else D3D12CreateDevice(mpHardwareAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mpDevice)); // Hardware

	return 0;
}
int GraphicsOutput::createCommandQueue() noexcept {
	
	// Create the command queue
	D3D12_COMMAND_QUEUE_DESC cqd = {};
	cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cqd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	mpDevice->CreateCommandQueue1(&cqd, __uuidof(ID3D12Device9), IID_PPV_ARGS(&mpCommandQueue));

	return 0;
}
int GraphicsOutput::checkTearingSupport() noexcept {
	mpFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mTearingSupport, sizeof(mTearingSupport));
	return 0;
}
int GraphicsOutput::createSwapChain(const HWND& hWnd, const UINT width, const UINT height) noexcept {
	
	// Create the swap chain
	DXGI_SWAP_CHAIN_DESC1 scd = {};
	scd.BufferCount = mBackBufferCount;
	scd.Width = width;
	scd.Height = height;
	scd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	scd.Stereo = FALSE;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.SampleDesc.Count = 1u;
	scd.SampleDesc.Quality = 0u;
	scd.Scaling = DXGI_SCALING_STRETCH;
	scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	if (mTearingSupport) scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
	scfd.RefreshRate.Numerator = 240u;
	scfd.RefreshRate.Denominator = 1u;
	scfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scfd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scfd.Windowed = TRUE;
	ComPtr<IDXGISwapChain1> refSwapChain{};
	mpFactory->CreateSwapChainForHwnd(mpCommandQueue.Get(), hWnd, &scd, &scfd, nullptr, refSwapChain.GetAddressOf());
	refSwapChain.As(&mpSwapChain);

	// No fullscreening support (change later)
	mpFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER); // DXGI_MWA_VALID

	return 0;
}
int GraphicsOutput::createRTV() noexcept {
	
	// Create Render Target View (RTV) heap
	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
	RTVHeapDesc.NumDescriptors = mBackBufferCount;
	RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	mpDevice->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mpRTVHeap));
	mRTVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(RTVHeapDesc.Type);

	// Create frame resources
	CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0u; i < mBackBufferCount; i++) {
		mpRenderTargets.push_back(*std::make_unique<ComPtr<ID3D12Resource2>>());
		mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&mpRenderTargets.at(i)));
		mpDevice->CreateRenderTargetView(mpRenderTargets[i].Get(), nullptr, hRTV);
		hRTV.Offset(1u, mRTVHeapSize);
	}
	return 0;
}
int GraphicsOutput::createCommandAllocatorAndList() noexcept {

	// Setup Graphics Commands
	for (int i = 0; i < mBackBufferCount; i++) {
		mpCommandAllocator.push_back(*std::make_unique<ComPtr<ID3D12CommandAllocator>>());
		mpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mpCommandAllocator[i]));
	}
	mpDevice->CreateCommandList1(0u, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&mpCommandList));
	return 0;
}
int GraphicsOutput::createFence() noexcept {
	
	// Create the Fence for synchronization
	mpDevice->CreateFence(0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence));
	mhFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	return 0;
}

// --Syncronization Methods--
UINT64 GraphicsOutput::signalFence(UINT64& fenceValue) noexcept {
	//Signal the fence
	UINT64 fenceSignalValue = fenceValue++;
	mHR = mpCommandQueue->Signal(mpFence.Get(), fenceSignalValue);
	return fenceSignalValue;
}
int GraphicsOutput::waitFence(UINT64 fenceValue) noexcept {
	// Wait for the fence (GPU Command List to complete)
	if (mpFence->GetCompletedValue() < fenceValue) {
		mHR = mpFence->SetEventOnCompletion(fenceValue, mhFenceEvent);
		WaitForSingleObject(mhFenceEvent, static_cast<DWORD>(INFINITE));
	}
	return 0;
}
int GraphicsOutput::flushGPU(UINT64& fenceValue) noexcept {
	UINT64 fenceSignalValue{ signalFence(fenceValue) };
	if (waitFence(fenceSignalValue) != 0) return 1;
	return 0;
}

// --Render Methods--
void GraphicsOutput::endFrame() {
	//DXGI_PRESENT_PARAMETERS pp = {};
	//pp.DirtyRectsCount = 0u;
	//pp.pDirtyRects = 0u;
	//pp.pScrollRect = 0u;
	//pp.pScrollOffset = 0u;
	//// Flip the Swap Chain
	//mpSwapChain->Present1(0u, 0u, &pp);
	//// Temporarily gain access to the Back Buffer
	//wrl::ComPtr<ID3D12Resource2> pBackBuffer;
	//mpSwapChain->GetBuffer(
	//	0,
	//	__uuidof(ID3D12Resource2),
	//	&pBackBuffer
	//);
	//// Now create the Render Target View and make it the Back Buffer
	//mpDevice->CreateRenderTargetView(
	//	pBackBuffer.Get(),
	//	&mpRenderTargets[mFrameIndex],
	//	mFenceEvent
	//);
	//mpCommandList->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView* const*>(m_pRenderTargetView.GetAddressOf()), m_pDepthStencilView.Get());
	
	//mpSwapChain->Present(4u, 0u);
}
void GraphicsOutput::flushBackBufferColor(float r, float g, float b) noexcept {
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
	//D3D12_CPU_DESCRIPTOR_HANDLE hDSV(mpRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	//mpCommandList->ClearDepthStencilView(hDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);
}
int GraphicsOutput::doRender() noexcept {

	//DXGI_QUERY_VIDEO_MEMORY_INFO vmi{};
	//mpHardwareAdapter->QueryVideoMemoryInfo(0u, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &vmi);
	//std::cout << vmi.Budget * 0.000001f << ' ' << vmi.CurrentUsage * 0.000001f << ' ' << '\n';
	Timer timer{};
	// Obtain a command allocator and render target view based on current frame index
	auto& pCurrentCommandAllocator = mpCommandAllocator[mFrameIndex];
	auto& pCurrentRenderTargetView = mpRenderTargets[mFrameIndex];

	pCurrentCommandAllocator->Reset();
	mpCommandList->Reset(pCurrentCommandAllocator.Get(), nullptr);

	/*mpCommandList->SetGraphicsRootSignature(mpRootSignature.Get());
	mpCommandList->RSSetViewports(1u, &mViewport);
	mpCommandList->RSSetScissorRects(1u, &mScissorRc);*/

	// Transition the current render target view from PRESENT state to RENDER_TARGET state for drawing
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(pCurrentRenderTargetView.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mpCommandList->ResourceBarrier(1u, &barrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
	//CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
	//mpCommandList->OMSetRenderTargets(1u, &hRTV, FALSE, &hDSV);

	auto color = std::make_unique<float[]>(4); color[0] = 0.5294f; color[1] = 0.8078f; color[2] = 0.9216f; color[3] = 1.f;

	mpCommandList->ClearRenderTargetView(hRTV, color.get(), 0u, nullptr);
	//mpCommandList->ClearDepthStencilView(hDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);
	//mpCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//mpCommandList->IASetVertexBuffers(0u, 1u, &mVertexBufferView);
	//mpCommandList->DrawInstanced(0u, 0u, 0u, 0u);

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mpCommandList->ResourceBarrier(1u, &barrier);
	mpCommandList->Close();

	ID3D12CommandList* ppCommandLists[] = {
		mpCommandList.Get()
	};

	mpCommandQueue->ExecuteCommandLists(1u, ppCommandLists);

	UINT syncInterval = mVSync ? 1u : 0u;
	UINT presentFlags = mTearingSupport && !mVSync ? DXGI_PRESENT_ALLOW_TEARING : 0u;
	mpSwapChain->Present(syncInterval, presentFlags);
	mFenceValues[mFrameIndex] = signalFence(mCurrentFenceValue);
	mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();
	waitFence(mFenceValues[mFrameIndex]);

	// For Benchmarking the render function
	/*sum += timer.mark() * 1000.f; runInstances++;
	if (timerBenchmark.peek() >= 30.f) {
		std::ofstream fileBenchmark{};
		fileBenchmark.open("benchmark.txt", std::ios::out | std::ios::app);
		fileBenchmark << sum / runInstances << " ms\n";
		timerBenchmark.mark();
	}*/

	return 0;
}

int GraphicsOutput::resizeWindow(UINT width, UINT height) noexcept {
	
	HWND hWnd{};
	mpSwapChain->GetHwnd(&hWnd);
	WINDOWINFO wndInfo{};
	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);
	
	LONG clientWidth{ wndInfo.rcWindow.right - wndInfo.rcWindow.left };
	LONG clientHeight{ wndInfo.rcWindow.bottom - wndInfo.rcWindow.top };
	
	if (clientWidth != width || clientHeight != height) {
		clientWidth = max(1u, width); clientHeight = max(1u, height);
		flushGPU(mCurrentFenceValue);
		for (int i = 0; i < mBackBufferCount; i++) {
			mpRenderTargets[i].Reset();
			mFenceValues[i] = mFenceValues[mFrameIndex];
		}
		DXGI_SWAP_CHAIN_DESC1 scd{};
		mpSwapChain->GetDesc1(&scd);
		mpSwapChain->ResizeBuffers(mBackBufferCount, clientWidth, clientHeight, scd.Format, scd.Flags);
		mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();
		createRTV();
	}
	return 0;
}
int GraphicsOutput::setFullscreen() noexcept {
	
	HWND hWnd{};
	mpSwapChain->GetHwnd(&hWnd);
	WINDOWINFO wndInfo{};
	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);

	if (!mFullscreen) {
		GetWindowRect(hWnd, &mWindowRc);
		UINT windowStyle{ WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX) };
		SetWindowLong(hWnd, GWL_STYLE, windowStyle);
		
		HMONITOR hMonitor{ MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) };
		MONITORINFOEX monitorInfo{};
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &monitorInfo);
		SetWindowPos(hWnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
			monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}
	else {
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_NOTOPMOST, mWindowRc.left, mWindowRc.top, mWindowRc.right - mWindowRc.left, mWindowRc.bottom - mWindowRc.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	
	mFullscreen = !mFullscreen;

	return 0;
}