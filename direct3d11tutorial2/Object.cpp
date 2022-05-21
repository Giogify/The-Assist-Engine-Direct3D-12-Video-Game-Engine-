#include "Object.h"
#include "IndexedTriangleList.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformCBuffer.h"
#include <DirectXMath.h>
#include <memory>

Object::Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data) {
	namespace dx = DirectX;

	m_object = std::make_unique<IndexedTriangleList::Object>(itl_data);

	std::vector<VertexData> ObjectData{};
	ObjectData.resize(m_object->pos.size());
	for (int i = 0; i < m_object->pos.size(); i++) {
		ObjectData.at(i).pos.x = m_object->pos.at(i).x;
		ObjectData.at(i).pos.y = m_object->pos.at(i).y;
		ObjectData.at(i).pos.z = m_object->pos.at(i).z;
		ObjectData.at(i).texcoord.x = m_object->tex.at(i).x;
		ObjectData.at(i).texcoord.y = m_object->tex.at(i).y;
		ObjectData.at(i).norm.x = m_object->norm.at(i).x;
		ObjectData.at(i).norm.y = m_object->norm.at(i).y;
		ObjectData.at(i).norm.z = m_object->norm.at(i).z;
	}

	m_MaterialData.colorDiffuse.r = itl_data.mtl.KD_RGB.at(0);
	m_MaterialData.colorDiffuse.g = itl_data.mtl.KD_RGB.at(1);
	m_MaterialData.colorDiffuse.b = itl_data.mtl.KD_RGB.at(2);
	m_MaterialData.colorDiffuse.a = 1.0f;

	m_MaterialData.colorEmissive.r = itl_data.mtl.KA_RGB.at(0);
	m_MaterialData.colorEmissive.g = itl_data.mtl.KA_RGB.at(1);
	m_MaterialData.colorEmissive.b = itl_data.mtl.KA_RGB.at(2);
	m_MaterialData.colorEmissive.a = 1.0f;

	m_MaterialData.colorSpecular.r = itl_data.mtl.KS_RGB.at(0);
	m_MaterialData.colorSpecular.g = itl_data.mtl.KS_RGB.at(1);
	m_MaterialData.colorSpecular.b = itl_data.mtl.KS_RGB.at(2);
	m_MaterialData.specularPower = itl_data.mtl.NS_exponent;

	for (int i = 0; i < 3; i++) {
		m_MaterialData.lightDirection[i].x = 1.0f;
		m_MaterialData.lightDirection[i].y = 1.0f;
		m_MaterialData.lightDirection[i].z = 1.0f;
		m_MaterialData.lightDirection[i].w = 1.0f;
	}

	for (int i = 0; i < 3; i++) {
		m_MaterialData.lightColorDiffuse[i].r = 1.0f;
		m_MaterialData.lightColorDiffuse[i].g = 1.0f;
		m_MaterialData.lightColorDiffuse[i].b = 1.0f;
		m_MaterialData.lightColorDiffuse[i].a = 1.0f;
	}

	for (int i = 0; i < 3; i++) {
		m_MaterialData.lightColorSpecular[i].r = 1.0f;
		m_MaterialData.lightColorSpecular[i].g = 1.0f;
		m_MaterialData.lightColorSpecular[i].b = 1.0f;
		m_MaterialData.lightColorSpecular[i].a = 1.0f;
	}

	m_MaterialData.eyePos.x = 1.0f;
	m_MaterialData.eyePos.y = 1.0f;
	m_MaterialData.eyePos.z = 1.0f;
	m_MaterialData.eyePos.w = 1.0f;

	m_MaterialData.fogColor.r = 1.0f;
	m_MaterialData.fogColor.g = 1.0f;
	m_MaterialData.fogColor.b = 1.0f;
	m_MaterialData.fogColor.a = 1.0f;

	m_MaterialData.fogVector.x = 1.0f;
	m_MaterialData.fogVector.y = 1.0f;
	m_MaterialData.fogVector.z = 1.0f;
	m_MaterialData.fogVector.w = 1.0f;

	addVertexBuffer(std::make_unique<VertexBuffer>(gfx, ObjectData.data(), ObjectData.size()));

	auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto pvsbc = pvs->getBytecode();
	addBind(std::move(pvs));

	addBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	addBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
	addBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(std::make_unique<TransformCBuffer>(gfx, *this));
}

void Object::update(float dt) noexcept {

}

DirectX::XMMATRIX Object::getTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pos.m_pitch, 0.0f, pos.m_roll)
		* DirectX::XMMatrixTranslation(pos.m_r, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(pos.m_theta, pos.m_phi, pos.m_chi)
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}