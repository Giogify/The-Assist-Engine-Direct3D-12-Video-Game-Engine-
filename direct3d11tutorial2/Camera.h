#pragma once
#include <DirectXMath.h>
#define _USE_MATH_DEFINES
#include <math.h>


class Camera {

public:

	float m_x{};
	float m_y{};
	float m_z{};
	float m_pitch{};
	float m_heading{};
	//DirectX::XMMATRIX m_matrix{};

public:

	Camera() {
		m_x = 0.0;
		m_y = 0.0;
		m_z = 0.0;
		m_pitch = 0.0;
		m_heading = 0.0;
	}

	/*
	Camera(DirectX::XMMATRIX matrix) :
		m_x(0), m_y(0), m_z(0), m_pitch(0), m_heading(0), m_matrix(matrix) {}*/

	DirectX::XMMATRIX getMatrix() const noexcept {
		//return m_matrix;
		return DirectX::XMMatrixRotationRollPitchYaw(m_pitch * (M_PI / 180), m_heading * (M_PI / 180), 0)
			* DirectX::XMMatrixTranslation(m_x, m_y, m_z);
	}
	
	/*
	void setMatrix(DirectX::XMMATRIX& matrix) noexcept {
		m_matrix = matrix;
	}
	*/

	void addHeading(float dh) noexcept {
		m_heading += dh;
	}

	void addPitch(float dp) noexcept {
		m_pitch += dp;
		if (m_pitch > 90.0) m_pitch = 90.0;
		if (m_pitch < -90.0) m_pitch = -90.0;
	}

	void translate(float x, float y, float z) noexcept {
		m_x += x;
		m_y += y;
		m_z += z;
	}
};