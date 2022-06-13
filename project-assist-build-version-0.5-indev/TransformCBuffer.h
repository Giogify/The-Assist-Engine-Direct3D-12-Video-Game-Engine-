//#pragma once
//#include "ConstantBuffer.h"
//#include "Light.h"
//#include "Object.h"
//#include "Timer.h"
//#include "Scene.h"
//#include <DirectXMath.h>
//
//class TransformCBuffer {
//
//public:
//
//	struct VertexCBuffer {
//		DirectX::XMMATRIX TransformXM{};
//		DirectX::XMMATRIX cameraMatrix{};
//		DirectX::XMMATRIX projMatrix{};
//		DirectX::XMMATRIX InverseTranspose{};
//	};
//
//	struct PixelCBuffer {
//		
//		//Object::MaterialData materialData;
//		
//		struct {
//			float x{};
//			float y{};
//			float z{};
//			float w{};
//		} eyePos;
//		
//		struct {
//			float r{};
//			float g{};
//			float b{};
//			float a{};
//		} globalAmbient;
//		
//		struct {
//			Light::LightData light{};
//		} lights[8];
//	};
//
//private:
//
//	//static std::unique_ptr<VertexConstantBuffer<VertexCBuffer>> mptrVCBuffer;
//	//static std::unique_ptr<PixelConstantBuffer<PixelCBuffer>> mptrPCBuffer;
//
//public:
//	
//	TransformCBuffer() = default;
//	TransformCBuffer(GraphicsOutput& gfx);
//	void bind(GraphicsOutput& gfx, std::unique_ptr<Scene>& scene) noexcept;
//
//};