#include "Object.h"
#include <DirectXMath.h>
#include <memory>

Object::Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data) {
	namespace dx = DirectX;

	m_object = *std::make_unique<IndexedTriangleList::Object>(itl_data);

	std::vector<VertexData> ObjectData{};
	ObjectData.resize(m_object.pos.size());
	for (int i = 0; i < m_object.pos.size(); i++) {
		ObjectData.at(i).pos.x = m_object.pos.at(i).x;
		ObjectData.at(i).pos.y = m_object.pos.at(i).y;
		ObjectData.at(i).pos.z = m_object.pos.at(i).z;
		ObjectData.at(i).texcoord.x = m_object.tex.at(i).x;
		ObjectData.at(i).texcoord.y = m_object.tex.at(i).y;
		ObjectData.at(i).norm.x = m_object.norm.at(i).x;
		ObjectData.at(i).norm.y = m_object.norm.at(i).y;
		ObjectData.at(i).norm.z = m_object.norm.at(i).z;
	}

	m_MaterialData.colorEmissive.r = 0.8f;
	m_MaterialData.colorEmissive.g = 0.8f;
	m_MaterialData.colorEmissive.b = 0.8f;
	m_MaterialData.colorEmissive.a = 1.0f;

	m_MaterialData.colorAmbient.r = 1.0f;
	m_MaterialData.colorAmbient.g = 1.0f;
	m_MaterialData.colorAmbient.b = 1.0f;
	m_MaterialData.colorAmbient.a = 1.0f;

	m_MaterialData.colorDiffuse.r = 0.8f;
	m_MaterialData.colorDiffuse.g = 0.8f;
	m_MaterialData.colorDiffuse.b = 0.8f;
	m_MaterialData.colorDiffuse.a = 1.0f;

	m_MaterialData.colorSpecular.r = 0.5f;
	m_MaterialData.colorSpecular.g = 0.5f;
	m_MaterialData.colorSpecular.b = 0.5f;
	m_MaterialData.colorSpecular.a = 1.0f;

	m_MaterialData.specularPower = 324.0f;
	m_MaterialData.isTextured = false;

	binds.vb = { gfx, ObjectData.data(), (unsigned int)ObjectData.size() };

	binds.vs = { gfx, L"VertexShader.cso" };
	auto pvsbc = binds.vs.getBytecode();

	binds.ps = { gfx, L"PixelShader.cso" };

	binds.ied = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	binds.inlay = { gfx, binds.ied, pvsbc };
	binds.top = { gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
}

void Object::update(float dt) noexcept {

}

DirectX::XMMATRIX Object::getTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pos.m_pitch, pos.m_yaw, pos.m_roll)
		* DirectX::XMMatrixTranslation(pos.m_r, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(pos.m_theta, pos.m_phi, pos.m_chi)
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}