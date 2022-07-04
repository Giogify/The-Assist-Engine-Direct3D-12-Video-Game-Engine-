/*
/  ----------------------------------------------
/  Mouse.h
/
/  A data system that handles the Mouse 
/  LeftMouseButton presses and releases, 
/  RightMouseButton presses and releases, mouse 
/  wheel events, mouse cursor movements, and 
/  additional mouse events.
/  
/  Currently no system for queuing mouse events 
/  for later interpretation.
/  ----------------------------------------------
*/
#pragma once
#include "PCWindows.h"
#include <queue>
namespace GID {
	namespace DSU {
		struct Mouse {

			// Event Class
			struct Event {

				enum class Type {

					LMBPress,
					LMBRelease,
					RMBPress,
					RMBRelease,
					WheelUp,
					WheelDown,
					Move,
					Enter,
					Exit,
					Invalid

				};

				Type	mType{};
				int16_t	mXPos{};
				int16_t	mYPos{};
				bool	mIsLMBPressed{};
				bool	mIsRMBPressed{};

				// Empty Constructor
				Event() noexcept :
					mType(Type::Invalid),
					mXPos(0),
					mYPos(0),
					mIsLMBPressed(false),
					mIsRMBPressed(false) {}

				// Expected Constructor
				Event(Type type, const Mouse& mouse) noexcept :
					mType(type),
					mXPos(mouse.mXPos),
					mYPos(mouse.mYPos),
					mIsLMBPressed(mouse.mIsLMBPressed),
					mIsRMBPressed(mouse.mIsRMBPressed) {}

				// Getters/Setters/Is
				Type getType() const noexcept { return mType; }
				bool isLMBPressed() const noexcept { return mIsLMBPressed; }
				bool isRMBPressed() const noexcept { return mIsRMBPressed; }
				int16_t getX() const noexcept { return mXPos; }
				int16_t getY() const noexcept { return mYPos; }
				bool isValid() const noexcept { return mType != Type::Invalid; }
				std::pair<int16_t, int16_t> getPos() const noexcept { return { mXPos, mYPos }; }

			};

			static constexpr uint16_t		MBUFFERSIZE{ USHRT_MAX };
			int16_t							mXPos{};
			int16_t							mYPos{};
			int16_t							mWheelDelta{ 0 };
			bool							mIsLMBPressed{ false };
			bool							mIsRMBPressed{ false };
			bool							mIsInsideWindow{ false };
			std::queue<Event>				mBuffer{};

			void onMouseMove(int16_t x, int16_t y) noexcept {
				mXPos = x;
				mYPos = y;

				mBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
				trimBuffer();
			}
			void onMouseEnter() noexcept {
				mIsInsideWindow = true;
				mBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
				trimBuffer();
			}
			void onMouseExit() noexcept {
				mIsInsideWindow = false;
				mBuffer.push(Mouse::Event(Mouse::Event::Type::Exit, *this));
				trimBuffer();
			}
			void onLMBPressed(int16_t x, int16_t y) noexcept {
				mIsLMBPressed = true;
				mBuffer.push(Mouse::Event(Mouse::Event::Type::LMBPress, *this));
				trimBuffer();
			}
			void onLMBReleased(int16_t x, int16_t y) noexcept {
				mIsLMBPressed = false;
				mBuffer.push(Mouse::Event(Mouse::Event::Type::LMBRelease, *this));
				trimBuffer();
			}
			void onRMBPressed(int16_t x, int16_t y) noexcept {
				mIsRMBPressed = true;
				mBuffer.push(Mouse::Event(Mouse::Event::Type::RMBPress, *this));
				trimBuffer();
			}
			void onRMBReleased(int16_t x, int16_t y) noexcept {
				mIsRMBPressed = false;
				mBuffer.push(Mouse::Event(Mouse::Event::Type::RMBRelease, *this));
				trimBuffer();
			}
			void onWheelUp(int16_t x, int16_t y) noexcept {
				mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
				trimBuffer();
			}
			void onWheelDown(int16_t x, int16_t y) noexcept {
				mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
				trimBuffer();
			}
			void onWheelDelta(int16_t x, int16_t y, int16_t delta) noexcept {
				mWheelDelta += delta;
				while (mWheelDelta >= WHEEL_DELTA) {
					mWheelDelta -= WHEEL_DELTA;
					onWheelUp(x, y);
				}
				while (mWheelDelta <= -(WHEEL_DELTA)) {
					mWheelDelta += WHEEL_DELTA;
					onWheelDown(x, y);
				}
			}
			void trimBuffer() noexcept {
				while (mBuffer.size() > MBUFFERSIZE) {
					mBuffer.pop();
				}
			}

			// Default Constructor
			Mouse() = default;

			// Getters/Setters/Is
			int16_t getX() const noexcept { return mXPos; }
			int16_t getY() const noexcept { return mYPos; }
			int16_t getWheelDelta() const noexcept { return mWheelDelta; }
			bool isLMBPressed() const noexcept { return mIsLMBPressed; }
			bool isRMBPressed() const noexcept { return mIsRMBPressed; }
			std::pair<int16_t, int16_t> getPos() const noexcept { return { mXPos,mYPos }; }
			bool isEmpty() const noexcept { return mBuffer.empty(); }

			// Read the Event
			Mouse::Event readEvent() noexcept {
				if (mBuffer.size() > 0u) {
					Mouse::Event e = mBuffer.front();
					mBuffer.pop();
					return e;
				}
				else { return Mouse::Event(); }
			}

			// Empty the Buffer
			void clear() noexcept { mBuffer = std::queue<Event>(); }

			// Is the Mouse in the Window
			bool isInsideWindow() const noexcept { return mIsInsideWindow; }

		};
	}
}