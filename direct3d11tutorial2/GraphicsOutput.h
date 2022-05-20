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
#include "Camera.h"

#include <d3d11_4.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>
#include <memory>

#pragma comment(lib, "D3DCompiler.lib")

class GraphicsOutput {

public: // Public Data Structures

	// for Direct 3D Exceptions
	class GraphicsException : public TestException {
		using TestException::TestException;
	};
	class GraphicsHRException : public GraphicsException {
	
	private: // Private Fields

		HRESULT hr;

	public: // Public Methods

		// Constructor
		GraphicsHRException(int throwLine, const char* file, HRESULT hr) noexcept;

		// Exception Methods
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT getErrorCode() const noexcept;
		std::string getErrorString() const noexcept;

	};
	class GraphicsRemovedDeviceException : public GraphicsHRException {
		
		using GraphicsHRException::GraphicsHRException;

	public: // Public Methods

		const char* getType() const noexcept override;
	
	};

private: // Private Fields

	Microsoft::WRL::ComPtr<ID3D11Device4>			m_pDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory4>			m_pFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4>			m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4>	m_pDeviceContext;
	//std::unique_ptr<Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>[], std::default_delete<Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>[]>> m_pRenderTargetViews = std::make_unique<Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>[]>(4u);
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>	m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;
	DirectX::XMMATRIX								m_projection = {};
	Camera											m_camera = {};

	const float MAX_DEPTH = 10.0f;

public: // Public Methods

	// Constructor
	GraphicsOutput(HWND hWnd);

	GraphicsOutput(const GraphicsOutput&) = delete;
	GraphicsOutput& operator=(const GraphicsOutput&) = delete;

	// Destructor
	~GraphicsOutput() = default;

	// General Graphics Operations
	void endFrame();
	void flushBackBufferColor(float r, float g, float b) noexcept;
	
	// Draw Graphics Operations
	void Draw(UINT count) noexcept;

	void setProjection(DirectX::XMMATRIX projection) noexcept;
	DirectX::XMMATRIX getProjection() const noexcept;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDeviceContext() const noexcept;
	Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const noexcept;
	void changeDepth(float delta) noexcept;
	float getDepth() const noexcept;

	Camera& getCamera() noexcept;
	//void setCamera(DirectX::XMMATRIX matrix) noexcept;

};