#pragma once
#define _USE_MATH_DEFINES

#include "PCWindows.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Timer.h"
#include <DirectXMath.h>
#include <math.h>
#include <memory>
#include <iostream>

class Camera {

private:

	DirectX::XMVECTOR X_ROTATION_AXIS = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR Y_ROTATION_AXIS = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR Z_ROTATION_AXIS = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

public:

	DirectX::XMFLOAT3 mEye{};
	DirectX::XMFLOAT3 mFocus{};

	float mRoll{};
	float mPitch{};
	float mYaw{};

	float mdx{};
	float mdy{};
	float mdz{};

	bool mIsInit{ false };
	bool mIsCameraModeEnabled{ false };

	Timer moveTimer{};
	Timer moveTimer2{};

public:

	Camera() = default;

	DirectX::XMMATRIX getMatrix() const noexcept {
		namespace DX = DirectX;

		DX::XMVECTOR eye = DX::XMVectorSet(mEye.x, mEye.y, mEye.z, 1.0f);
		DX::XMVECTOR focus = DX::XMVectorSet(mFocus.x, mFocus.y, mFocus.z, 1.0f);
		DX::XMVECTOR up = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		DX::XMMATRIX cameraMatrix = DX::XMMatrixLookAtLH(eye, focus, up);

		cameraMatrix *= DX::XMMatrixRotationAxis(Z_ROTATION_AXIS, DX::XMConvertToRadians(mRoll));
		cameraMatrix *= DX::XMMatrixRotationAxis(X_ROTATION_AXIS, DX::XMConvertToRadians(mPitch));
		cameraMatrix *= DX::XMMatrixRotationAxis(Y_ROTATION_AXIS, DX::XMConvertToRadians(mYaw));

		return cameraMatrix;
	}
	void addPitch(float delta) noexcept {
		mPitch += delta;
		if (mPitch > 90.0) mPitch = 90.0;
		if (mPitch < -90.0) mPitch = -90.0;

		/*DirectX::XMFLOAT4 Y_AXIS{};
		DirectX::XMStoreFloat4(&Y_AXIS, Y_ROTATION_AXIS);
		Y_AXIS.z -= delta;
		Y_ROTATION_AXIS = DirectX::XMLoadFloat4(&Y_AXIS);*/

	}
	void addYaw(float delta) noexcept {
		mYaw += delta;
		while (mYaw > 360) mYaw -= 360;
		while (mYaw < -360) mYaw += 360;
	}
	void translate(float dx, float dy, float dz) noexcept {
		if (!mIsInit) {
			mEye.x += dx;
			mEye.y += dy;
			mEye.z += dz;
			mIsInit = true;
		}
		else {
			mEye.x += dx;
			mEye.y += dy;
			mEye.z += dz;
			mFocus.x += dx;
			mFocus.y += dy;
			mFocus.z += dz;
		}
	}
	void input(const Keyboard& kb, const std::vector<Keyboard::Event>& keys, const std::vector<unsigned char>& keysChar, 
		const Mouse& mouse, const std::vector<Mouse::Event>& mouseEvents, const RECT& rc) noexcept {

		float dt = moveTimer2.peek();

		if (dt >= 0.001f) {
			while (dt >= 0.001f) {
				if (kb.KeyIsPressed('W')) mdz -= 0.025f;
				if (kb.KeyIsPressed('S')) mdz += 0.025f;
				if (kb.KeyIsPressed('A')) mdx += 0.025f;
				if (kb.KeyIsPressed('D')) mdx -= 0.025f;
				dt -= 0.001f;
			}
			moveTimer2.mark();
		}

		if (mIsCameraModeEnabled) {
			for (auto& e : mouseEvents) {
				if (e.getType() == Mouse::Event::Type::Move) {
					auto centerX = (rc.right - rc.left) / 2;
					auto centerY = (rc.bottom - rc.top) / 2;
					if (e.getX() != centerX || e.getY() != centerY) {
						SetCursorPos(centerX + rc.left, centerY + rc.top);
					}
					auto dx = centerX - e.getX();
					auto dy = centerY - e.getY();
					addYaw(0.05f * dx);
					addPitch(0.05f * dy);
				}
			}
		}

		for (auto& e : mouseEvents) if (e.getType() == Mouse::Event::Type::LMBPress) {
			mIsCameraModeEnabled = !mIsCameraModeEnabled;
			ShowCursor(!mIsCameraModeEnabled);
			if (mIsCameraModeEnabled) ClipCursor(&rc);
			else { ClipCursor(NULL); }
			auto centerX = (rc.right - rc.left) / 2;
			auto centerY = (rc.bottom - rc.top) / 2;
			SetCursorPos(centerX + rc.left, centerY + rc.top);
		}

	}
	
	int update() noexcept {
		translate(mdx, mdy, mdz);

		std::cout << "[X Rotation] " << X_ROTATION_AXIS.m128_f32[0] << " "
			<< X_ROTATION_AXIS.m128_f32[1] << " "
			<< X_ROTATION_AXIS.m128_f32[2] << " "
			<< X_ROTATION_AXIS.m128_f32[3]
			<< '\n';

		std::cout << "[Y Rotation] " << Y_ROTATION_AXIS.m128_f32[0] << " "
			<< Y_ROTATION_AXIS.m128_f32[1] << " "
			<< Y_ROTATION_AXIS.m128_f32[2] << " "
			<< Y_ROTATION_AXIS.m128_f32[3]
			<< '\n';

		std::cout << "[Z Rotation] " << Z_ROTATION_AXIS.m128_f32[0] << " "
			<< Z_ROTATION_AXIS.m128_f32[1] << " "
			<< Z_ROTATION_AXIS.m128_f32[2] << " "
			<< Z_ROTATION_AXIS.m128_f32[3]
			<< '\n';

		//std::cout << "[X] " << mdx << " " << " [Y] " << mdy << " " << " [Z] " << mdz << '\n';

		float dt = moveTimer.peek();

		if (dt >= 0.001f) {
			while (dt >= 0.001f) {
				if (mdx < 0.0001f && mdx > -0.0001f) mdx = 0.f;
				mdx = mdx / 1.01f;

				if (mdy < 0.0001f && mdy > -0.0001f) mdy = 0.f;
				mdy = mdy / 1.01f;

				if (mdz < 0.0001f && mdz > -0.0001f) mdz = 0.f;
				mdz = mdz / 1.01f;

				dt -= 0.001f;
			}
			moveTimer.mark();
		}
		return 0;
	}
};