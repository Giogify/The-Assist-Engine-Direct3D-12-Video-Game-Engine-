#pragma once
#define _USE_MATH_DEFINES

#include "PCWindows.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Timer.h"
#include "DataStructures.h"

#include <DirectXMath.h>
#include <math.h>
#include <memory>
#include <iostream>

class Camera {

public:

	enum FollowMode {
		NO_FOLLOW,
		FOLLOW
	};

private:

	DirectX::XMVECTOR Y_AXIS = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

public:

	DirectX::XMFLOAT3 mEye{};
	DirectX::XMFLOAT3 mFocus{};

	float mRoll{};
	float mPitch{};
	float mYaw{};

	float mdx{};
	float mdy{};
	float mdz{};

	bool mbMouseControl{ false };
	bool mbFollow{ false };

	Timer moveTimer{};
	Timer moveTimer2{};

public:

	Camera() = default;
	Camera(BOOL bFollow) : mbFollow(bFollow) {
		mEye = { 0.0f, 0.0f, -5.0f };
	}

	DirectX::XMMATRIX getMatrix() const noexcept {
		namespace DX = DirectX;
		DX::XMVECTOR eye = DX::XMVectorSet(mEye.x, mEye.y, mEye.z, 1.0f);
		//DX::XMVECTOR focus = DX::XMVectorSet(mFocus.x, mFocus.y, mFocus.z, 1.0f);
		DX::XMVECTOR look = DX::XMVectorSet(
			std::sin(DX::XMConvertToRadians(mYaw)) * std::cos(DX::XMConvertToRadians(mPitch)),
			std::sin(DX::XMConvertToRadians(mPitch)),
			std::cos(DX::XMConvertToRadians(mYaw)) * std::cos(DX::XMConvertToRadians(mPitch)),
			1.0f
		);
		DX::XMMATRIX cameraMatrix = DX::XMMatrixLookToLH(eye, look, Y_AXIS);
		return cameraMatrix;
	}
	void addPitch(float delta) noexcept {
		mPitch += delta;
		while (mPitch > 360) mPitch -= 360;
		while (mPitch < -360) mPitch += 360;

		/*DirectX::XMFLOAT4 Y_AXIS{};
		DirectX::XMStoreFloat4(&Y_AXIS, Y_ROTATION_AXIS);
		Y_AXIS.z -= delta;
		Y_ROTATION_AXIS = DirectX::XMLoadFloat4(&Y_AXIS);*/

	}
	void addYaw(float delta) noexcept {
		mYaw -= delta;
		while (mYaw > 360) mYaw -= 360;
		while (mYaw < -360) mYaw += 360;
	}
	void translate(float dx, float dy, float dz) noexcept {
		mEye.x += dx;
		mEye.y += dy;
		mEye.z += dz;
		mFocus.x += dx;
		mFocus.y += dy;
		mFocus.z += dz;
	}
	void input(const Keyboard& kb, const std::vector<Keyboard::Event>& keys, const std::vector<unsigned char>& keysChar, 
		const Mouse& mouse, const std::vector<Mouse::Event>& mouseEvents, const RECT& rc) noexcept {

		if (!mbFollow) {
			BOOL shift = kb.KeyIsPressed(VK_SHIFT);
			shift++;
			float dt = moveTimer2.mark();

			if (kb.KeyIsPressed('W')) {
				float xcom = std::sin(DirectX::XMConvertToRadians(mYaw));
				float zcom = std::cos(DirectX::XMConvertToRadians(mYaw));
				mdx += dt * xcom * shift;
				mdz += dt * zcom * shift;
			}
			if (kb.KeyIsPressed('S')) {
				float xcom = std::sin(DirectX::XMConvertToRadians(mYaw));
				float zcom = std::cos(DirectX::XMConvertToRadians(mYaw));
				mdx += dt * -xcom * shift;
				mdz += dt * -zcom * shift;
			}
			if (kb.KeyIsPressed('A')) {
				float xcom = std::sin(DirectX::XMConvertToRadians(mYaw - 90.0f));
				float zcom = std::cos(DirectX::XMConvertToRadians(mYaw - 90.0f));
				mdx += dt * xcom * shift;
				mdz += dt * zcom * shift;
			}
			if (kb.KeyIsPressed('D')) {
				float xcom = std::sin(DirectX::XMConvertToRadians(mYaw + 90.0f));
				float zcom = std::cos(DirectX::XMConvertToRadians(mYaw + 90.0f));
				mdx += dt * xcom * shift;
				mdz += dt * zcom * shift;
			}
			if (kb.KeyIsPressed(VK_SPACE)) mdy += dt * 1.0f * shift;
			if (kb.KeyIsPressed(VK_TAB)) mdy -= dt * 1.0f * shift;

			if (kb.KeyIsPressed(VK_LEFT)) addYaw(0.25f);
			if (kb.KeyIsPressed(VK_RIGHT)) addYaw(-0.25f);
			if (kb.KeyIsPressed(VK_UP)) addPitch(0.25f);
			if (kb.KeyIsPressed(VK_DOWN)) addPitch(-0.25f);

			if (mbMouseControl) {
				BOOL control = kb.KeyIsPressed(VK_CONTROL);
				control++;
				for (auto& e : mouseEvents) {
					if (e.getType() == Mouse::Event::Type::Move) {
						auto centerX = (rc.right - rc.left) / 2;
						auto centerY = (rc.bottom - rc.top) / 2;
						if (e.getX() != centerX || e.getY() != centerY) SetCursorPos(centerX + rc.left, centerY + rc.top);
						auto dx = centerX - e.getX();
						auto dy = centerY - e.getY();
						addYaw(0.05f * dx * control);
						addPitch(0.05f * dy * control);
					}
				}
			}

			for (auto& e : mouseEvents) if (e.getType() == Mouse::Event::Type::LMBPress) {
				mbMouseControl = !mbMouseControl;
				ShowCursor(!mbMouseControl);
				if (mbMouseControl) ClipCursor(&rc);
				else { ClipCursor(NULL); }
				auto centerX = (rc.right - rc.left) / 2;
				auto centerY = (rc.bottom - rc.top) / 2;
				SetCursorPos(centerX + rc.left, centerY + rc.top);
			}
		}
	}
	
	int update() noexcept {
		if (!mbFollow) {
			translate(mdx, mdy, mdz);
			//std::cout << "[X] " << mdx << " " << " [Y] " << mdy << " " << " [Z] " << mdz << '\n';
			if (mdx < 0.0001f && mdx > -0.0001f) mdx = 0.0f;
			else mdx /= 1.1f;
			if (mdy < 0.0001f && mdy > -0.0001f) mdy = 0.0f;
			else mdy /= 1.1f;
			if (mdz < 0.0001f && mdz > -0.0001f) mdz = 0.0f;
			else mdz /= 1.1f;
		}
		return 0;
	}
};