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
		float m_r{};
		float m_roll{};
		float m_pitch{};
		float m_yaw{};
		float m_theta{};
		float m_phi{};
		float m_chi{};
	};

	// Speed Fields
	struct Speed {
		float m_droll{};
		float m_dpitch{};
		float m_dyaw{};
		float m_dtheta{};
		float m_dphi{};
		float m_dchi{};
	};

private:

	Binds binds{};

	Position pos{};
	Speed speed{};

	// Object Data
	IndexedTriangleList::Object m_object{};
	MaterialData m_MaterialData{};

public:

	Object() = default;

	Object(const Object& obj) : 
	pos(obj.pos),
	speed(obj.speed),
	m_object(obj.m_object),
	m_MaterialData(obj.m_MaterialData),
	binds(obj.binds) {}

	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);

	void update(float dt) noexcept;

	DirectX::XMMATRIX getTransformXM() const noexcept;

	Position& getPos() noexcept { return pos; }

	Speed& getSpeed() noexcept { return speed; }

	MaterialData& getMaterialData() noexcept { return m_MaterialData; }

	void draw(GraphicsOutput& gfx) noexcept {
		binds.vb.bind(gfx);
		binds.vs.bind(gfx);
		binds.ps.bind(gfx);
		binds.inlay.bind(gfx);
		binds.top.bind(gfx);
		gfx.Draw(binds.vb.getCount());
	}

};