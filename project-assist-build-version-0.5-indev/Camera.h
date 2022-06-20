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

	enum FollowMode {
		NO_FOLLOW,
		FOLLOW
	};

private:

	AssistMath::AMVECTOR Y_AXIS{ 0.0f, 1.0f, 0.0f, 1.0f };

public:

	AssistMath::AMDOUBLE3 mEye{ 0.0f, 0.0f, -5.0f };
	AssistMath::AMDOUBLE3 mFocus{ 0.0f, 0.0f, 0.0f };

	double mRoll{};
	double mPitch{};
	double mYaw{};

	double mdx{};
	double mdy{};
	double mdz{};

	bool mbMouseControl{ false };
	bool mbFollow{ false };

	Timer moveTimer{};
	Timer moveTimer2{};

public:

	Camera() = default;

	AssistMath::AMMATRIX getMatrix() const noexcept {
		using namespace AssistMath;
		AMMATRIX cameraMatrix{};
		if (mbFollow) {
			AMVECTOR eye{ mEye.x, mEye.y, mEye.z, 1.0f };
			AMVECTOR focus{ mFocus.x, mFocus.y, mFocus.z, 1.0f };
			return { AMMatrixLookAtLH(eye, focus, Y_AXIS) };
		}
		if (!mbFollow) {
			AMVECTOR eye{ mEye.x, mEye.y, mEye.z, 1.0f };
			//AMVECTOR focus = DX::XMVectorSet(mFocus.x, mFocus.y, mFocus.z, 1.0f);
			AMVECTOR look{
				std::sin(AMConvertToRadians(mYaw)) * std::cos(AMConvertToRadians(mPitch)),
				std::sin(AMConvertToRadians(mPitch)),
				std::cos(AMConvertToRadians(mYaw)) * std::cos(AMConvertToRadians(mPitch)),
				1.0f
			};
			return { AMMatrixLookToLH(eye, look, Y_AXIS) };
		}
	}
	void addPitch(double delta) noexcept {
		mPitch += delta;
		if (mPitch > 90.0f) mPitch = 89.999f;
		if (mPitch < -90.0f) mPitch = -89.999f;
	}
	void addYaw(double delta) noexcept {
		mYaw -= delta;
		while (mYaw > 360) mYaw -= 360;
		while (mYaw < -360) mYaw += 360;
	}
	void translate(double dx, double dy, double dz) noexcept {
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
				double xcom = std::sin(AMConvertToRadians(mYaw));
				double zcom = std::cos(AMConvertToRadians(mYaw));
				mdx += 0.00025f * -xcom * shift;
				mdz += 0.00025f * -zcom * shift;
			}
			if (kb.KeyIsPressed('A')) {
				double xcom = std::sin(AMConvertToRadians(mYaw - 90.0f));
				double zcom = std::cos(AMConvertToRadians(mYaw - 90.0f));
				mdx += 0.00025f * xcom * shift;
				mdz += 0.00025f * zcom * shift;
			}
			if (kb.KeyIsPressed('D')) {
				double xcom = std::sin(AMConvertToRadians(mYaw + 90.0f));
				double zcom = std::cos(AMConvertToRadians(mYaw + 90.0f));
				mdx += 0.00025f * xcom * shift;
				mdz += 0.00025f * zcom * shift;
			}
			if (kb.KeyIsPressed(VK_SPACE)) mdy += 0.00025f * shift;
			if (kb.KeyIsPressed(VK_TAB)) mdy -= 0.00025f * shift;

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
			else mdx /= 1.02f;
			if (mdy < 0.0001f && mdy > -0.0001f) mdy = 0.0f;
			else mdy /= 1.02f;
			if (mdz < 0.0001f && mdz > -0.0001f) mdz = 0.0f;
			else mdz /= 1.02f;
		}
		return 0;
	}

};