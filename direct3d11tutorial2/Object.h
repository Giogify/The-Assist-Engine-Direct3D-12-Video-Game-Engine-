#pragma once
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "IndexedTriangleList.h"

class Object {

	friend class Model;

public:

	// Binds
	struct Binds {
		VertexBuffer vb{};
		VertexShader vs{};
		PixelShader ps{};
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied{};
		InputLayout inlay{};
		Topology top{};
	};

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

	// Material Data
	struct MaterialData {
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorEmissive;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorAmbient;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorDiffuse;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorSpecular;

		float specularPower{};
		int isTextured{ false };
		int padding0{};
		int padding1{};
	};

	// Positional Fields
	struct Position {
		float r{};
		float roll{};
		float pitch{};
		float yaw{};
		float theta{};
		float phi{};
		float chi{};
	};

	// Speed Fields
	struct Speed {
		float droll{};
		float dpitch{};
		float dyaw{};
		float dtheta{};
		float dphi{};
		float dchi{};
	};

private:

	Binds mBinds{};

	Position mPos{};
	Speed mSpeed{};

	// Object Data
	IndexedTriangleList::Object mObject{};
	MaterialData mMaterialData{};

public:

	Object() = default;

	Object(const Object& obj) : 
	mPos(obj.mPos),
	mSpeed(obj.mSpeed),
		mObject(obj.mObject),
		mMaterialData(obj.mMaterialData),
		mBinds(obj.mBinds) {}

	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);

	void update(float dt) noexcept;

	DirectX::XMMATRIX getTransformXM() const noexcept;

	Position& getPos() noexcept { return mPos; }

	Speed& getSpeed() noexcept { return mSpeed; }

	MaterialData& getMaterialData() noexcept { return mMaterialData; }

	void draw(GraphicsOutput& gfx) noexcept {
		mBinds.vb.bind(gfx);
		mBinds.vs.bind(gfx);
		mBinds.ps.bind(gfx);
		mBinds.inlay.bind(gfx);
		mBinds.top.bind(gfx);
		gfx.Draw(mBinds.vb.getCount());
	}

};