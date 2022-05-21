#include "GraphicsOutput.h"
#include <ios>
#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#define GRAPHICS_THROW_FAILED(hrcall) if (FAILED(hr = (hrcall))) throw GraphicsOutput::GraphicsHRException(__LINE__,__FILE__,hr)
#define GRAPHCS_REMOVED_DEVICE_EXCEPT(hr) GraphicsOutput::GraphicsRemovedDeviceException(__LINE__,__FILE__,(hr))

// Exceptions
GraphicsOutput::GraphicsHRException::GraphicsHRException(int throwLine, const char* file, HRESULT hr) noexcept :
	GraphicsException(throwLine, file),
	hr(hr) {}
const char* GraphicsOutput::GraphicsHRException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << '\n'
		<< "[ERROR CODE] - 0x" << std::hex << std::uppercase << getErrorCode()
		<< std::dec << " (" << (unsigned long)getErrorCode() << ")" << '\n'
		<< "[ERROR STRING] - " << getErrorString() << '\n'
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* GraphicsOutput::GraphicsHRException::getType() const noexcept { return "[Graphics Exception]: [General]"; }
HRESULT GraphicsOutput::GraphicsHRException::getErrorCode() const noexcept{ return hr; }
std::string GraphicsOutput::GraphicsHRException::getErrorString() const noexcept {
	std::string* pMsgBuffer{};
	
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM 
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		getErrorCode(),
		0,
		(LPTSTR) &pMsgBuffer,
		0,
		NULL
	);

	return *pMsgBuffer;
}
const char* GraphicsOutput::GraphicsRemovedDeviceException::getType() const noexcept { return "[Graphics Exception]: [Removed Device]"; }

GraphicsOutput::GraphicsOutput(HWND hWnd) {

	CreateDXGIFactory2(
		0u,
		__uuidof(IDXGIFactory4),
		&m_pFactory
	);

	// Instantiate and Populate the Swap Chain Descriptor
	DXGI_SWAP_CHAIN_DESC1 sd = {};
	sd.Width = 0u;
	sd.Height = 0u;
	sd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	sd.Scaling = DXGI_SCALING_STRETCH;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2u;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.Flags = 0;

	// HRESULT Check
	HRESULT hr{};

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenSCDesc = {};
	fullscreenSCDesc.RefreshRate.Numerator = 240u;
	fullscreenSCDesc.RefreshRate.Denominator = 1u;
	fullscreenSCDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullscreenSCDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fullscreenSCDesc.Windowed = TRUE;

	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0u,
		D3D11_SDK_VERSION,
		reinterpret_cast<ID3D11Device**>(m_pDevice.GetAddressOf()),
		nullptr,
		reinterpret_cast<ID3D11DeviceContext**>(m_pDeviceContext.GetAddressOf())
	);

	m_pFactory->CreateSwapChainForHwnd(
		m_pDevice.Get(),
		hWnd,
		&sd,
		&fullscreenSCDesc,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(m_pSwapChain.GetAddressOf())
	);

	// Initialize the Front and Back Frame Buffers, along with Swap Chain and the Device Context
	/*D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceContext
	);*/

	// Temporarily gain access to the Back Buffer
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Resource),
		&pBackBuffer
	);

	// Now create the Render Target View and make it the Back Buffer
	m_pDevice->CreateRenderTargetView1(
		pBackBuffer.Get(),
		nullptr,
		&m_pRenderTargetView
	);

	// Create a Depth Buffer (State & Texture)

	// Configure Depth Stencil State
	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &pDepthStencilState);
	m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	// Configure Depth Stencil Texture
	wrl::ComPtr<ID3D11Texture2D1> pDepthStencilTexture;

	D3D11_TEXTURE2D_DESC1 depthStencilTextureDesc = {};
	depthStencilTextureDesc.Width = 1280u;
	depthStencilTextureDesc.Height = 720u;
	depthStencilTextureDesc.MipLevels = 1u;
	depthStencilTextureDesc.ArraySize = 1u;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilTextureDesc.SampleDesc.Count = 1u;
	depthStencilTextureDesc.SampleDesc.Quality = 0u;
	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	
	m_pDevice->CreateTexture2D1(&depthStencilTextureDesc, nullptr, &pDepthStencilTexture);

	// Generate View of the Depth Stencil Texture
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	m_pDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), &depthStencilViewDesc, &m_pDepthStencilView);
	m_pDeviceContext->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView* const*>(m_pRenderTargetView.GetAddressOf()), m_pDepthStencilView.Get());

	// Configure Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 1280;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1u, &vp);

	Camera m_camera = {};

	// Create Sampler

	//auto samplerBorderColor = std::make_unique<float[]>(4);
	//for (int i = 0; i < 4; i++) samplerBorderColor[i] = 1.0f;

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1u;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	wrl::ComPtr<ID3D11SamplerState> samplerState;

	m_pDevice->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
	m_pDeviceContext->PSSetSamplers(0u, 1u, samplerState.GetAddressOf());
}

void GraphicsOutput::endFrame() {

	DXGI_PRESENT_PARAMETERS pp = {};
	pp.DirtyRectsCount = 0u;
	pp.pDirtyRects = 0u;
	pp.pScrollRect = 0u;
	pp.pScrollOffset = 0u;

	// Flip the Swap Chain
	m_pSwapChain->Present1(1u, 0u, &pp);

	// Temporarily gain access to the Back Buffer
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Resource),
		&pBackBuffer
	);

	// Now create the Render Target View and make it the Back Buffer
	m_pDevice->CreateRenderTargetView1(
		pBackBuffer.Get(),
		nullptr,
		&m_pRenderTargetView
	);
	m_pDeviceContext->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView* const*>(m_pRenderTargetView.GetAddressOf()), m_pDepthStencilView.Get());

}
void GraphicsOutput::flushBackBufferColor(float r, float g, float b) noexcept {
	const float color[] = { r, g, b, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void GraphicsOutput::Draw(UINT count) noexcept {

	// Draw Command
	m_pDeviceContext->Draw(count, 0u);
}

// Getters/Setters/Is
void GraphicsOutput::setProjection(DirectX::XMMATRIX projection) noexcept {
	m_projection = projection;
}
DirectX::XMMATRIX GraphicsOutput::getProjection() const noexcept {
	return m_projection;
}
Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsOutput::getDeviceContext() const noexcept {
	return m_pDeviceContext;
}
Microsoft::WRL::ComPtr<ID3D11Device> GraphicsOutput::getDevice() const noexcept {
	return m_pDevice;
}
void GraphicsOutput::changeDepth(float delta) noexcept {
	dx::XMFLOAT4X4 proj_matrix{};
	dx::XMStoreFloat4x4(&proj_matrix, m_projection);
	if (delta > 0) {
		float newValue = (proj_matrix._11 / 2) + delta;
		if (newValue <= MAX_DEPTH) {
			setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, newValue, 500.0f));
			return;
		}
		else {
			setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, MAX_DEPTH, 500.0f));
		}
	}
	if (delta < 0) {
		float newValue = (proj_matrix._11 / 2) + delta;
		if (newValue > 0.0f) {
			setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, newValue, 500.0f));
			return;
		}
		return;
	}
}
float GraphicsOutput::getDepth() const noexcept {
	dx::XMFLOAT4X4 proj_matrix{};
	dx::XMStoreFloat4x4(&proj_matrix, m_projection);
	return (proj_matrix._11 / 2);
}

Camera& GraphicsOutput::getCamera() noexcept {
	return m_camera;
}
//void GraphicsOutput::setCamera(DirectX::XMMATRIX matrix) noexcept {
//	m_camera.setMatrix(matrix);
//}