#pragma once
#define _USE_MATH_DEFINES

#include "PCWindows.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Timer.h"
#include "DataStructures.h"
#include "Scriptable.h"
#include "AssistMath.h"

#include <math.h>
#include <memory>
#include <iostream>

class Camera : public Scriptable {

public:

	enum Rotation {
		Pitch,
		Yaw,
		Roll
	};

	enum Translation {
		X,
		Y,
		Z
	};

	enum FollowMode {
		NO_FOLLOW,
		FOLLOW
	};

private:

	AssistMath::FAMVECTOR Y_AXIS{ 0.0f, 1.0f, 0.0f, 0.0f };

public:

	AssistMath::FAMVECTOR mEye{ 0.0f, 0.0f, -5.0f, 1.0f };
	AssistMath::FAMVECTOR mFocus{ 0.0f, 0.0f, 0.0f, 0.0f };

	AssistMath::FAMVECTOR mRotation{};
	AssistMath::FAMVECTOR mDTranslation{};

	bool mbMouseControl{ false };
	bool mbFollow{ false };

	Timer moveTimer{};
	Timer moveTimer2{};

public:

	Camera() = default;

	AssistMath::FAMMATRIX getMatrix() noexcept {
		using namespace AssistMath;
		if (mbFollow) {
			return { FAMMatrixLookAtLH(mEye, mFocus, Y_AXIS) };
		}
		if (!mbFollow) {
			//AMVECTOR focus = DX::XMVectorSet(mFocus.x, mFocus.y, mFocus.z, 1.0f);
			FAMVECTOR rotationRad{ _mm_mul_ps(mRotation, _mm_set_ps1((float)AM_PI / 180.0f)) };
			FAMVECTOR cosvec{};
			FAMVECTOR sinvec{ _mm_sincos_ps(&cosvec, rotationRad) };
			FAMVECTOR look{
				sinvec.m128_f32[Rotation::Yaw] * cosvec.m128_f32[Rotation::Pitch],
				sinvec.m128_f32[Rotation::Pitch],
				cosvec.m128_f32[Rotation::Yaw] * cosvec.m128_f32[Rotation::Pitch],
				0.0f
			};
			return { FAMMatrixLookToLH(mEye, look, Y_AXIS) };
		}
	}
	void addPitch(float delta) noexcept {
		mRotation.m128_f32[Rotation::Pitch] += delta;
		if (mRotation.m128_f32[Rotation::Pitch] > 90.0f) mRotation.m128_f32[Rotation::Pitch] = 89.999f;
		if (mRotation.m128_f32[Rotation::Pitch] < -90.0f) mRotation.m128_f32[Rotation::Pitch] = -89.999f;
	}
	void addYaw(float delta) noexcept {
		mRotation.m128_f32[Rotation::Yaw] -= delta;
		while (mRotation.m128_f32[Rotation::Yaw] > 360) mRotation.m128_f32[Rotation::Yaw] -= 360;
		while (mRotation.m128_f32[Rotation::Yaw] < -360) mRotation.m128_f32[Rotation::Yaw] += 360;
	}
	void translate(AssistMath::FAMVECTOR& translation) noexcept {
		mEye = _mm_add_ps(mEye, translation);
		mFocus = _mm_add_ps(mFocus, translation);
	}
	void input(const Keyboard& kb, const std::vector<Keyboard::Event>& keys, const std::vector<unsigned char>& keysChar, 
		const Mouse& mouse, const std::vector<Mouse::Event>& mouseEvents, const RECT& rc) noexcept {

		if (!mbFollow) {
			using namespace AssistMath;
			BOOL shift = kb.KeyIsPressed(VK_SHIFT);
			shift++;

			/*if (kb.KeyIsPressed('W')) {
				float xcom = std::sin(DirectX::XMConvertToRadians(mYaw));
				float zcom = std::cos(DirectX::XMConvertToRadians(mYaw));
				mdx += 0.00025f * xcom * shift;
				mdz += 0.00025f * zcom * shift;
			}*/
			if (kb.KeyIsPressed('S')) {
				float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
				float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
				mDTranslation.m128_f32[Translation::X] += 0.00025f * -xcom * shift;
				mDTranslation.m128_f32[Translation::Z] += 0.00025f * -zcom * shift;
			}
			if (kb.KeyIsPressed('A')) {
				double xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] - 90.0f));
				double zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] - 90.0f));
				mDTranslation.m128_f32[Translation::X] += 0.00025f * xcom * shift;
				mDTranslation.m128_f32[Translation::Z] += 0.00025f * zcom * shift;
			}
			if (kb.KeyIsPressed('D')) {
				double xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] + 90.0f));
				double zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] + 90.0f));
				mDTranslation.m128_f32[Translation::X] += 0.00025f * xcom * shift;
				mDTranslation.m128_f32[Translation::Z] += 0.00025f * zcom * shift;
			}
			if (kb.KeyIsPressed(VK_SPACE)) mDTranslation.m128_f32[Translation::Y] += 0.00025f * shift;
			if (kb.KeyIsPressed(VK_TAB)) mDTranslation.m128_f32[Translation::Y] -= 0.00025f * shift;

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
			translate(mDTranslation);
			//std::cout << "[X] " << mdx << " " << " [Y] " << mdy << " " << " [Z] " << mdz << '\n';
			if (mDTranslation.m128_f32[Translation::X] < 0.0001f 
				&& mDTranslation.m128_f32[Translation::X] > -0.0001f) 
				mDTranslation.m128_f32[Translation::X] = 0.0f;
			
			else mDTranslation.m128_f32[Translation::X] /= 1.02f;
			
			if (mDTranslation.m128_f32[Translation::Y] < 0.0001f 
				&& mDTranslation.m128_f32[Translation::Y] > -0.0001f) 
				mDTranslation.m128_f32[Translation::Y] = 0.0f;
			
			else mDTranslation.m128_f32[Translation::Y] /= 1.02f;
			
			if (mDTranslation.m128_f32[Translation::Z] < 0.0001f 
				&& mDTranslation.m128_f32[Translation::Z] > -0.0001f) 
				mDTranslation.m128_f32[Translation::Z] = 0.0f;
			
			else mDTranslation.m128_f32[Translation::Z] /= 1.02f;
		}
		return 0;
	}

};