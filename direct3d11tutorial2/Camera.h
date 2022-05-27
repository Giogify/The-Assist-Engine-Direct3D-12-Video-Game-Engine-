#pragma once
#include "PCWindows.h"
#include "Mouse.h"
#include <DirectXMath.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>


class Camera {

private:

	const DirectX::XMVECTOR X_ROTATION_AXIS = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR Y_ROTATION_AXIS = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR Z_ROTATION_AXIS = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

public:

	DirectX::XMFLOAT3 mEye{};
	DirectX::XMFLOAT3 mFocus{};

	float mRoll{};
	float mPitch{};
	float mYaw{};

	bool mIsInit{ false };
	bool mIsCameraModeEnabled{ false };

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
	void input(std::vector<char>& keys, std::vector<Mouse::Event>& mouse, RECT rc) noexcept {
		
		for (auto& key : keys) {
			if (key == 'w') translate(0.0, 0.5, 0.0);
			if (key == 's') translate(0.0, -0.5, 0.0);
			if (key == 'a') translate(0.5, 0.0, 0.0);
			if (key == 'd') translate(-0.5, 0.0, 0.0);
		}

		for (auto& e : mouse) {
			if (e.getType() == Mouse::Event::Type::WheelUp) translate(0.0, 0.0, -0.5);
			if (e.getType() == Mouse::Event::Type::WheelDown) translate(0.0, 0.0, 0.5);
		}

		if (mIsCameraModeEnabled) {
			for (auto& e : mouse) {
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

		for (auto& e : mouse) if (e.getType() == Mouse::Event::Type::LMBPress) {
			mIsCameraModeEnabled = !mIsCameraModeEnabled;
			ShowCursor(!mIsCameraModeEnabled);
			if (mIsCameraModeEnabled) ClipCursor(&rc);
			else { ClipCursor(NULL); }
			auto centerX = (rc.right - rc.left) / 2;
			auto centerY = (rc.bottom - rc.top) / 2;
			SetCursorPos(centerX + rc.left, centerY + rc.top);
		}

	}
};