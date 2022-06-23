#include "GraphicsOutput.h"

#include <ios>
#include <sstream>
#include <d3dcompiler.h>
#include <array>
#include <fstream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")

namespace wrl = Microsoft::WRL;

GraphicsOutput::GraphicsOutput(HWND& hWnd) {

	WINDOWINFO wndInfo{};
	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);

	UINT16 width = wndInfo.rcClient.right - wndInfo.rcClient.left;
	UINT16 height = wndInfo.rcClient.bottom - wndInfo.rcClient.top;

#if defined(_DEBUG)
	mDebug = true;
#endif

	parseGraphicsConfig();

	if (mDebug) {
		std::cout << "[Graphics Pipeline | Initialization]\n";
		std::cout << "[Init Info] Using Debug Compile Flags for Shaders\n";
		std::cout << "[Init Info] Back Buffer Count: #" << mBackBufferCount << '\n';
		if (!mUseWARPAdapter) std::cout << "[Init Info] Expected Adapter Type: Hardware\n";
		if (mUseWARPAdapter) std::cout << "[Init Info] Expected Adapter Type: WARP\n";
		std::cout << "[Init Info] Minimum Feature Level: 11_0\n";
	}

	// Create the viewport
	mViewport = { CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)width, (FLOAT)height) };
	mScissorRc = { CD3DX12_RECT(0u, 0u, LONG_MAX, LONG_MAX) };

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
	std::thread thread8([this, width, height] { createDSV(width, height); });
	thread4.join();
	std::thread thread9([this, hWnd, width, height] { createSwapChain(hWnd, width, height); });
	std::thread thread10([this] { createCommandAllocatorAndList(); });
	thread9.join();
	std::thread thread11([this] { createRTV(); });
	thread1.join(); thread5.join(); thread6.join(); thread7.join(); thread8.join(); thread10.join(); thread11.join();

	initializePipeline();
	flushGPU();
}

// --Creation Methods--
void GraphicsOutput::parseGraphicsConfig() noexcept {

	enum GFXConfigDataType {
		UIBackBufferCount,
		BUseWARPAdapter,
		BVSync,
		BFullScreenOnStartup,
		BEnableWireframe,
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
			else if (strCurrentLine.starts_with("BEnableWireframe")) currentData = BEnableWireframe;
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
			case BEnableWireframe:
				strCurrentLine.erase(0u, 18u);
				if (strCurrentLine == "TRUE") mWireframe = true;
			case Invalid:
				if (mDebug) std::cout << "[WARNING] Invalid line parsed from \"gfx_config.txt\"\n"
					<< "[LINE] " << strCurrentLine;
				break;
			}
			strCurrentLine.clear();
		}
	}
}
void GraphicsOutput::createDebugLayer() noexcept {
	// Create the Debug Interface
	if (mDebug) {
		D3D12GetDebugInterface(IID_PPV_ARGS(&mpDebugController));
		mpDebugController->EnableDebugLayer();
		mpDebugController->SetEnableAutoName(TRUE);
	}
}
void GraphicsOutput::createInfoQueue() noexcept {
	if (mDebug) {
		mpDevice.As(&mpInfoQueue);
		mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, TRUE);
	}
}
void GraphicsOutput::createFactory() noexcept {
	// Create factory
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mpFactory));
}
void GraphicsOutput::enumerateAdapters() noexcept {
	// Obtain adapter
	if (mUseWARPAdapter) mpFactory->EnumWarpAdapter(IID_PPV_ARGS(&mpWARPAdapter)); // WARP
	else {
		UINT64 maxVideoMem = 0;
		ComPtr<IDXGIAdapter1> refAdapter{};
		for (UINT i = 0; mpFactory->EnumAdapters1(i, refAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++) {
			DXGI_ADAPTER_DESC1 ad{};
			refAdapter->GetDesc1(&ad);
			if ((ad.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && D3D12CreateDevice(refAdapter.Get(), mFeatureLevel, __uuidof(ID3D12Device9), nullptr)
				&& ad.DedicatedVideoMemory > maxVideoMem) { maxVideoMem = ad.DedicatedVideoMemory; refAdapter.As(&mpHardwareAdapter); }
		}
	}
}
void GraphicsOutput::createDevice() noexcept {
	// Create the Device
	if (mUseWARPAdapter) D3D12CreateDevice(mpWARPAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mpDevice)); // WARP
	else D3D12CreateDevice(mpHardwareAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mpDevice)); // Hardware
	mpDevice->SetName(L"[Core] [ID3D12Device9] Member of GraphicsOutput");
}
void GraphicsOutput::createCommandQueue() noexcept {
	// Create the command queue
	D3D12_COMMAND_QUEUE_DESC cqd = {};
	cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cqd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	mpDevice->CreateCommandQueue1(&cqd, __uuidof(ID3D12Device9), IID_PPV_ARGS(&mpCommandQueue));
	mpCommandQueue->SetName(L"[Command] [ID3D12CommandQueue] Member of GraphicsOutput");
}
void GraphicsOutput::checkTearingSupport() noexcept {
	mpFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mTearingSupport, sizeof(mTearingSupport));
}
void GraphicsOutput::createSwapChain(const HWND& hWnd, const UINT16& width, const UINT16& height) noexcept {
	// Get window and monitor info
	HMONITOR hMonitor{ MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) };
	MONITORINFOEX monitorInfo{};
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &monitorInfo);
	DEVMODE devMode{};
	devMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
	// Create the swap chain
	DXGI_SWAP_CHAIN_DESC1 scd = {}; {
		scd.BufferCount = mBackBufferCount;
		scd.Width = width; scd.Height = height;
		scd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		scd.Stereo = FALSE;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scd.SampleDesc.Count = 1u;
		scd.SampleDesc.Quality = 0u;
		scd.Scaling = DXGI_SCALING_STRETCH;
		scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		if (mTearingSupport) scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {}; {
		scfd.RefreshRate.Numerator = devMode.dmDisplayFrequency;
		scfd.RefreshRate.Denominator = 1u;
		scfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scfd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}
	if (mFullscreen) scfd.Windowed = FALSE; else scfd.Windowed = TRUE;
	ComPtr<IDXGISwapChain1> refSwapChain{};
	mpFactory->CreateSwapChainForHwnd(mpCommandQueue.Get(), hWnd, &scd, &scfd, nullptr, refSwapChain.GetAddressOf());
	refSwapChain.As(&mpSwapChain);
	// Manual fullscreen support
	mpFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER); // DXGI_MWA_VALID
}
void GraphicsOutput::createRTV() noexcept {
	// Create Render Target View (RTV) heap
	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {}; {
		RTVHeapDesc.NumDescriptors = mBackBufferCount;
		RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}
	mpDevice->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mpRTVHeap));
	mRTVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(RTVHeapDesc.Type);
	mpRTVHeap->SetName(L"[RTV] [ID3D12DescriptorHeap] Member of GraphicsOutput");
	// Create frame resources
	CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0u; i < mBackBufferCount; i++) {
		mpRenderTargets.push_back(*std::make_unique<ComPtr<ID3D12Resource2>>());
		mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&mpRenderTargets.at(i)));
		mpDevice->CreateRenderTargetView(mpRenderTargets[i].Get(), nullptr, hRTV);
		std::wstringstream woss{};
		woss << "[RTV] [ID3D12Resource2] [Render Target View #" << i + 1 << "]";
		mpRenderTargets[i]->SetName(woss.str().c_str());
		hRTV.Offset(1u, mRTVHeapSize);
	}

}
void GraphicsOutput::createDSV(const UINT16& width, const UINT16& height) noexcept {
	// Create Render Target View (RTV) heap
	D3D12_DESCRIPTOR_HEAP_DESC dsvhd{}; {
		dsvhd.NumDescriptors = 1u;
		dsvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		mpDevice->CreateDescriptorHeap(&dsvhd, IID_PPV_ARGS(&mpDSVHeap));
		mDSVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(dsvhd.Type);
	}
	D3D12_CLEAR_VALUE dscv{}; {
		dscv.Format = DXGI_FORMAT_D32_FLOAT;
		dscv.DepthStencil = { 1.0f, 0 };
	}
	// Configure Depth Stencil Texture
	CD3DX12_HEAP_PROPERTIES DSTexHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	auto DSTexHeapDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height);
	DSTexHeapDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	mHR = mpDevice->CreateCommittedResource(&DSTexHeapProperties, D3D12_HEAP_FLAG_NONE, &DSTexHeapDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &dscv,
		IID_PPV_ARGS(&mpDepthStencilTexture));
	mpDepthStencilTexture->SetName(L"[DSV] [ID3D12Resource2] [DepthStencilTexture]");
}
void GraphicsOutput::createCommandAllocatorAndList() noexcept {
	// Setup Graphics Commands
	for (int i = 0; i < mBackBufferCount; i++) {
		mpCommandAllocator.push_back(*std::make_unique<ComPtr<ID3D12CommandAllocator>>());
		mpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mpCommandAllocator[i]));
		std::wstringstream woss{};
		woss << "[Command] [ID3D12CommandAllocator] [Command Allocator #" << i + 1 << "]";
		mpCommandAllocator[i]->SetName(woss.str().c_str());
	}
	mpDevice->CreateCommandList1(0u, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&mpCommandList));
	mpCommandList->SetName(L"[Command] [ID3D12GraphicsCommandList6] [Command List]");

	mpCommandAllocator[0]->Reset();
	mpCommandList->Reset(mpCommandAllocator[0].Get(), nullptr);
}
void GraphicsOutput::createFence() noexcept {
	// Create the Fence for synchronization
	mpDevice->CreateFence(0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence));
	mpFence->SetName(L"[Sync] [ID3D12Fence1] [Fence]");
	mhFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}
void GraphicsOutput::initializePipeline() noexcept {
	D3DReadFileToBlob(L"VertexShader.cso", &mpVSBytecode);
	D3DReadFileToBlob(L"PixelShader.cso", &mpPSBytecode);
	std::array<D3D12_INPUT_ELEMENT_DESC, 3u> ied{}; {
		ied[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		ied[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		ied[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	}
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{}; {
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	}
	D3D12_ROOT_SIGNATURE_FLAGS rsFlags = {
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS };
	std::array<CD3DX12_ROOT_PARAMETER1, 2u> rp{}; {
		rp[0].InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
		rp[1].InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_PIXEL);
	}
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rsd{}; {
		rsd.Init_1_1(rp.size(), rp.data(), 0u, nullptr, rsFlags);
		D3DX12SerializeVersionedRootSignature(&rsd, featureData.HighestVersion, &mpSignatureRootBlob, &mpErrorBlob);
		mpDevice->CreateRootSignature(0u, mpSignatureRootBlob->GetBufferPointer(), mpSignatureRootBlob->GetBufferSize(), IID_PPV_ARGS(&mpRootSignature));
		mpRootSignature->SetName(L"[Shaders] [Root Signature]");
	}
	D3D12_RT_FORMAT_ARRAY RTVfarr{}; {
		RTVfarr.NumRenderTargets = 1u;
		RTVfarr.RTFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	}
	D3D12_PIPELINE_STATE_STREAM_DESC pssd{}; {
		mPSS.pRootSignature = mpRootSignature.Get();
		mPSS.InputLayout = { ied.data(), (UINT)ied.size() };
		mPSS.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		mPSS.VS = CD3DX12_SHADER_BYTECODE(mpVSBytecode.Get());
		mPSS.PS = CD3DX12_SHADER_BYTECODE(mpPSBytecode.Get());
		mPSS.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		mPSS.RTVFormats = RTVfarr;
		if (mDebug) {
			if (mWireframe) {
				mPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
					D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
			}
			else {
				mPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
					D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
			}
		}
		else mPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pssd.pPipelineStateSubobjectStream = &mPSS;
		pssd.SizeInBytes = sizeof(DSU::PipelineStateStream);
		mpDevice->CreatePipelineState(&pssd, IID_PPV_ARGS(&mpPipelineState));
		mpPipelineState->SetName(L"[Pipeline State]");
	}
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv{}; {
		dsv.Format = DXGI_FORMAT_D32_FLOAT;
		dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv.Texture2D.MipSlice = 0u;
		dsv.Flags = D3D12_DSV_FLAG_NONE;
		CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
		mpDevice->CreateDepthStencilView(mpDepthStencilTexture.Get(), &dsv, hDSV);
	}
}

// --Syncronization Methods--
void GraphicsOutput::signalFence() noexcept {
	//Signal the fence
	mHR = mpCommandQueue->Signal(mpFence.Get(), ++mFenceValue);
}
int GraphicsOutput::waitFence() noexcept {
	// Wait for the fence
	if (mpFence->GetCompletedValue() < mFenceValue) {
		mpFence->SetEventOnCompletion(mFenceValue, mhFenceEvent);
		WaitForSingleObject(mhFenceEvent, (DWORD)INFINITE);
	}
	return 0;
}
int GraphicsOutput::flushGPU() noexcept {
	mpCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { mpCommandList.Get() };
	mpCommandQueue->ExecuteCommandLists(1u, ppCommandLists);
	signalFence();
	waitFence();
	mpCommandAllocator[mFrameIndex]->Reset();
	mpCommandList->Reset(mpCommandAllocator[mFrameIndex].Get(), nullptr);
	return 0;
}

// --Prepare Methods--
void GraphicsOutput::transitionRTVToRead() noexcept {
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mpCommandList->ResourceBarrier(1u, &barrier);
}
void GraphicsOutput::transitionRTVToWrite() noexcept {
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mpCommandList->ResourceBarrier(1u, &barrier);
}
void GraphicsOutput::clearRTV() noexcept {
	CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
	auto color = std::make_unique<float[]>(4); color[0] = 0.5294f; color[1] = 0.8078f; color[2] = 0.9216f; color[3] = 1.f;
	//auto color = std::make_unique<float[]>(4); color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.f;
	mpCommandList->ClearRenderTargetView(hRTV, color.get(), 0u, nullptr);
}
void GraphicsOutput::clearDSV() noexcept {
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
	mpCommandList->ClearDepthStencilView(hDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);
}
void GraphicsOutput::setRenderTarget() noexcept {
	CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
	mpCommandList->OMSetRenderTargets(1u, &hRTV, FALSE, &hDSV);
}

// --Render Methods--
void GraphicsOutput::startFrame() noexcept {
	transitionRTVToWrite();
	clearRTV();
	clearDSV();
	setRenderTarget();
	mpCommandList->SetGraphicsRootSignature(mpRootSignature.Get());
	mpCommandList->SetPipelineState(mpPipelineState.Get());
	mpCommandList->RSSetViewports(1u, &mViewport);
	mpCommandList->RSSetScissorRects(1u, &mScissorRc);
}
void GraphicsOutput::doFrame() noexcept {

	//const std::array<DSU::PositionData, 8u> vertices{
	//	DX::XMFLOAT3(-1.0f, -1.0f, -1.0f),
	//	DX::XMFLOAT3(-1.0f, 1.0f, -1.0f),
	//	DX::XMFLOAT3(1.0f, 1.0f, -1.0f),
	//	DX::XMFLOAT3(1.0f, -1.0f, -1.0f),
	//	DX::XMFLOAT3(-1.0f, -1.0f, 1.0f),
	//	DX::XMFLOAT3(-1.0f, 1.0f, 1.0f),
	//	DX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	//	DX::XMFLOAT3(1.0f, -1.0f, 1.0f),
	//};
	//const std::array<WORD, 36u> indices{
	//	0, 1, 2, 0, 2, 3,
	//	4, 6, 5, 4, 7, 6,
	//	4, 5, 1, 4, 1, 0,
	//	3, 2, 6, 3, 6, 7,
	//	1, 5, 6, 1, 6, 2,
	//	4, 0, 3, 4, 3, 7
	//};

	//DX::XMMATRIX transformM{
	//	DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
	//	* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
	//	* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
	//	* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
	//};
	//auto InvDeterminant = DirectX::XMMatrixDeterminant(transformM);
	//auto InvM = DirectX::XMMatrixTranspose(transformM);
	//DSU::VertexConstantBuffer mx{ transformM, mCamera.getMatrix(), DX::XMMatrixPerspectiveLH(1.f, 9.0f / 16.0f, 0.25f, 5000.f), 
	//	DirectX::XMMatrixInverse(&InvDeterminant, InvM) };

	//// Add resources
	//VertexBuffer vb(mpDevice, mpCommandList, vertices.data(), vertices.size());
	//IndexBuffer ib(mpDevice, mpCommandList, indices.data(), indices.size());
	//VertexConstantBuffer vcb(mpDevice, mpCommandList, mx);
	//vb.transitionToRead(mpCommandList);
	//ib.transitionToRead(mpCommandList);
	//vcb.transitionToRead(mpCommandList);
	//mpCommandList->SetGraphicsRootConstantBufferView(0u, vcb.getDestRes()->GetGPUVirtualAddress());

	//mpCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//mpCommandList->IASetVertexBuffers(0u, 1u, &vb.getView());
	//mpCommandList->IASetIndexBuffer(&ib.getView());
	//mpCommandList->DrawIndexedInstanced(ib.getCount(), 1u, 0u, 0u, 0u);
	//endFrame();
}
void GraphicsOutput::endFrame() noexcept {
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
	transitionRTVToRead();
	flushGPU();
	UINT syncInterval = mVSync ? 1u : 0u;
	UINT presentFlags = mTearingSupport && !mVSync ? DXGI_PRESENT_ALLOW_TEARING : 0u;
	mpSwapChain->Present(syncInterval, presentFlags);
	mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();
}

int GraphicsOutput::resizeWindow(UINT width, UINT height) noexcept {
	
	/*HWND hWnd{};
	mpSwapChain->GetHwnd(&hWnd);
	WINDOWINFO wndInfo{};
	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);
	
	LONG clientWidth{ wndInfo.rcWindow.right - wndInfo.rcWindow.left };
	LONG clientHeight{ wndInfo.rcWindow.bottom - wndInfo.rcWindow.top };
	
	if (clientWidth != width || clientHeight != height) {
		clientWidth = max(1u, width); clientHeight = max(1u, height);
		flushGPU();
		for (int i = 0; i < mBackBufferCount; i++) {
			mpRenderTargets[i].Reset();
		}
		DXGI_SWAP_CHAIN_DESC1 scd{};
		mpSwapChain->GetDesc1(&scd);
		mpSwapChain->ResizeBuffers(mBackBufferCount, clientWidth, clientHeight, scd.Format, scd.Flags);
		mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();
		createRTV();
	}*/
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