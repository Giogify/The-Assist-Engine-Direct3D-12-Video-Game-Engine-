#pragma once
#include "DrawableBase.h"
#include "IndexedTriangleList.h"

class Object : public DrawableBase<Object> {

	friend class Model;

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

	// Material Data
	struct MaterialData {
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
		} colorEmissive;
		struct {
			float r{};
			float g{};
			float b{};
		} colorSpecular;

		float specularPower{};

		struct {
			float x;
			float y;
			float z;
			float w;
		} lightDirection[3];
		struct {
			float r;
			float g;
			float b;
			float a;
		} lightColorDiffuse[3];
		struct {
			float r;
			float g;
			float b;
			float a;
		} lightColorSpecular[3];
		struct {
			float x;
			float y;
			float z;
			float w;
		} eyePos;
		struct {
			float r;
			float g;
			float b;
			float a;
		} fogColor;
		struct {
			float x;
			float y;
			float z;
			float w;
		} fogVector;
	};

	// Positional Fields
	struct Position {
		float m_r{ 0.0f };
		float m_roll{ 0.0f };
		float m_pitch{ 0.0f };
		float m_yaw{ 0.0f };
		float m_theta{ 0.0f };
		float m_phi{ 0.0f };
		float m_chi{ 0.0f };
	};

	// Speed Fields
	struct Speed {
		float m_droll{ 0.0f };
		float m_dpitch{ 0.0f };
		float m_dyaw{ 0.0f };
		float m_dtheta{ 0.0f };
		float m_dphi{ 0.0f };
		float m_dchi{ 0.0f };
	};

private:

	Position pos{};
	Speed speed{};

	// Object Data
	std::unique_ptr<IndexedTriangleList::Object> m_object = nullptr;
	MaterialData m_MaterialData{};

public:

	//Object(const Object&) = delete;

	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);

	void update(float dt) noexcept override;

	DirectX::XMMATRIX getTransformXM() const noexcept override;

	Position& getPos() noexcept { return pos; }

	Speed& getSpeed() noexcept { return speed; }

	MaterialData& getMaterialData() noexcept { return m_MaterialData; }

};