#pragma once
#include <DirectXMath.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>


class Camera {

private:

	struct Position {
		float x{};
		float y{};
		float z{};
	};

private:

	const DirectX::XMVECTOR X_ROTATION_AXIS = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR Y_ROTATION_AXIS = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR Z_ROTATION_AXIS = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

public:

	Position mEye{};
	Position mFocus{};

	float mRoll{};
	float mPitch{};
	float mYaw{};

	bool mIsInit{ false };

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

		//DX::XMVECTOR scalingOrigin{ 0.0f, 0.0f, 0.0f, 1.0f };
		//DX::XMVECTOR scaling{ 1.0f, 1.0f, 1.0f, 1.0f };
		//DX::XMVECTOR rotationOrigin{ 0.0f, 0.0f, 0.0f, 1.0f };
		//DX::XMVECTOR translation{ mEye.x, mEye.y, mEye.z, 1.0f };

		//return DX::XMMatrixTransformation(
		//	//Scaling Origin
		//	scalingOrigin,
		//	//Scaling Orientation
		//	DX::XMQuaternionRotationRollPitchYaw(DX::XMConvertToRadians(mPitch), DX::XMConvertToRadians(mYaw), DX::XMConvertToRadians(mRoll)),
		//	//Scaling
		//	scaling,
		//	//Rotation Origin
		//	rotationOrigin,
		//	//Rotation Orientation
		//	DX::XMQuaternionRotationRollPitchYaw(DX::XMConvertToRadians(mPitch), DX::XMConvertToRadians(mYaw), DX::XMConvertToRadians(mRoll)),
		//	//Translation
		//	translation
		//);
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
			mFocus.y += dy;
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
};